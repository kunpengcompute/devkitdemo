package io.jenkins.plugins.devkit;

import edu.umd.cs.findbugs.annotations.NonNull;
import hudson.*;
import hudson.model.*;
import hudson.remoting.VirtualChannel;
import hudson.tasks.BuildStepDescriptor;
import hudson.tasks.Builder;
import hudson.util.FormValidation;


import io.jenkins.plugins.devkit.utils.BashCmdExec;
import jenkins.tasks.SimpleBuildStep;

import org.jenkinsci.Symbol;
import org.kohsuke.stapler.DataBoundConstructor;
import org.kohsuke.stapler.QueryParameter;

import java.io.File;
import java.io.IOException;
import java.io.Serializable;

import javax.servlet.ServletException;

import io.jenkins.plugins.devkit.utils.ParameterFmt;
import io.jenkins.plugins.devkit.slavecopy.CopyToSlaveBuildWrapper;

public class PortingAdvisorBuilder extends Builder implements SimpleBuildStep, Serializable {
    private static final long serialVersionUID = 1L;
    private final String prefix;
    private final String command;
    private final String hardwareArchitecture;
    private final String scantype;
    private final String scanpath;
    private final String compiler;
    private final String cmd;
    private final String targetos;
    private final String extendedparameter;
    private String newCommand;
    private ParameterFmt parameterFmt = new ParameterFmt();

    @DataBoundConstructor
    public PortingAdvisorBuilder(String command, String hardwareArchitecture, String scantype, String scanpath,
                                 String compiler, String cmd, String targetos, String extendedparameter, String prefix) {
        this.prefix = prefix.trim();
        this.scantype = scantype.trim();
        this.scanpath = scanpath.trim();
        this.compiler = compiler.trim();
        this.cmd = cmd.trim();
        this.targetos = targetos.trim();
        this.extendedparameter = extendedparameter.trim();
        this.hardwareArchitecture = hardwareArchitecture;
        this.parameterFmt.ParameterFmt(command, this.hardwareArchitecture, this.scantype, this.scanpath,
                this.compiler, this.cmd, this.targetos, this.extendedparameter, this.prefix);
        this.command = command;
    }

    public String initCmd(TaskListener listener) {
        return this.parameterFmt.toStringCmd(listener);
    }
    public static long getSerialVersionUID() {
        return serialVersionUID;
    }
    public String getHardwareArchitecture() {
        return hardwareArchitecture;
    }

    public String getScantype() {
        return scantype;
    }

    public String getScanpath() {
        return scanpath;
    }

    public String getCompiler() {
        return compiler;
    }

    public String getCmd() {
        return cmd;
    }

    public String getTargetos() {
        return targetos;
    }

    public String getExtendedparameter() {
        return extendedparameter;
    }
    public String getPrefix() {
        return prefix;
    }

    @Override
    public void perform(@NonNull Run<?, ?> run, @NonNull FilePath workspace, @NonNull EnvVars env, Launcher launcher,
                        @NonNull TaskListener listener) throws InterruptedException, IOException {
        listener.getLogger().println("workspace value= " + workspace);

        this.newCommand = this.parameterFmt.toStringCmd(listener);
        VirtualChannel channel = launcher.getChannel();
        assert channel != null;
        try {
            listener.getLogger().println("start to copyToSlave");
            new CopyToSlaveBuildWrapper().copyToSlave(run, launcher, listener, new FilePath(new File(this.prefix)));
            channel.call(
                    new CommandMasterToSlaveCallable(this.newCommand, listener, workspace.getParent(), this.prefix));
        } catch (IOException e ) {
            listener.getLogger().println("[error]" + e);
            Util.displayIOException(e, listener);
            run.setResult(Result.FAILURE);
        }
    }

    private class CommandMasterToSlaveCallable extends jenkins.security.MasterToSlaveCallable<Void,IOException>
        implements Serializable {
        private static final long serialVersionUID = 1L;
        private Job<?, ?> project;
        private String prefix;
        private String command;
        private TaskListener listener;
        private FilePath workspace;

        CommandMasterToSlaveCallable(String command, TaskListener listener,
                                     FilePath workspace, String prefix) {
            this.command =command.trim();
            this.listener = listener;
            this.prefix = prefix.trim();
            this.workspace = workspace;
        }



        @Override
        public Void call() throws IOException {
            try {
                // 在执行命令前，判断文件是否存在，不存在则进行copy处理
                listener.getLogger().println("[install] Installing package ...");
                PortingAdvisorInstall PAInstall = new PortingAdvisorInstall();
                FilePath installTarget;
                if (this.prefix.length() == 0) {
                    listener.getLogger().println("[INFO] Prefix is empty. will be installed in the /opt");
                }
                installTarget = this.prefix.length() > 0 ? new FilePath(new File(this.prefix)) : new FilePath(new File("/opt"));
                listener.getLogger().println("[workspace] is " +  workspace);
                listener.getLogger().println("installTarget is: " + installTarget);
                PAInstall.install(listener, parameterFmt.getHardwareArchitectures(), workspace, installTarget);
                BashCmdExec.callProcessBuilder(this.command, listener);
            }catch (Exception e) {
                throw new IOException(e.getMessage());
            }
            return null;
        }
    }

    @Extension
    @Symbol("porting_advisor")
    public static class DescriptorImpl extends BuildStepDescriptor<Builder> {

        public FormValidation doCheckName(@QueryParameter String value, @QueryParameter boolean useFrench)
            throws IOException, ServletException {
            if (value.length() == 0) {
                return FormValidation.error(Messages.PortingAdvisorBuilder_DescriptorImpl_errors_missingName());
            }
            if (value.length() < 4) {
                return FormValidation.warning(Messages.PortingAdvisorBuilder_DescriptorImpl_warnings_tooShort());
            }
            if (!useFrench && value.matches(".*[éáàç].*")) {
                return FormValidation.warning(Messages.PortingAdvisorBuilder_DescriptorImpl_warnings_reallyFrench());
            }
            return FormValidation.ok();
        }

        @Override
        public boolean isApplicable(Class<? extends AbstractProject> aClass) {
            return true;
        }

        @Override
        public String getDisplayName() {
            return Messages.PortingAdvisorBuilder_DescriptorImpl_DisplayName();
        }

    }


}
