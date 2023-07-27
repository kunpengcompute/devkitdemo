package io.jenkins.plugins.devkit;

import edu.umd.cs.findbugs.annotations.NonNull;
import hudson.*;
import hudson.model.*;
import hudson.remoting.VirtualChannel;
import hudson.tasks.BuildStepDescriptor;
import hudson.tasks.Builder;
import hudson.util.FormValidation;


import io.jenkins.plugins.devkit.utils.BashCmdExec;
import io.jenkins.plugins.devkit.webserver.PortadvAffinityAnalysis;
import io.jenkins.plugins.devkit.webserver.PortadvBinaryProcess;
import io.jenkins.plugins.devkit.webserver.SourceCodeMigration;
import io.jenkins.plugins.devkit.webserver.SrvType;
import jenkins.tasks.SimpleBuildStep;

import net.sf.json.JSONObject;
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
    private final String targetkernel;
    private final String serviceType;
    private final String service;
    private final String webserverIp;
    private final String webserverPort;
    private final String constructtool;
    private final String gfortran;
    private final String customizedmacros;
    private final Boolean interpreted;
    private final Boolean sourceenhancecheck;
    private final String instpath;
    private final String compilecommand;
    private final String osMappingDir;
    private final String gllvm;
    private final String autoFix;
    private final String compilerType;
    private final String enableSVE;
    private final String username;
    private final String password;
    @DataBoundConstructor
    public PortingAdvisorBuilder(String command, String hardwareArchitecture, String scantype, String scanpath,
                                 String compiler, String cmd, String targetos, String extendedparameter, String prefix,
                                 String targetkernel, String serviceType, String service, String gfortran,
                                 String customizedmacros, Boolean interpreted, Boolean sourceenhancecheck, String instpath,
                                 String webserverIp, String webserverPort, String constructtool, String compilecommand,
                                 String osMappingDir, String gllvm, String autoFix, String compilerType,
                                 String enableSVE, String username, String password) {
        this.hardwareArchitecture = hardwareArchitecture;
        this.prefix = prefix;
        this.scantype = scantype;
        this.scanpath = scanpath;
        this.compiler = compiler;
        this.cmd = cmd;
        this.targetos = targetos;
        this.extendedparameter = extendedparameter;
        this.constructtool = constructtool;
        this.gfortran = gfortran;
        this.customizedmacros = customizedmacros;
        this.interpreted = interpreted;
        this.sourceenhancecheck = sourceenhancecheck;
        this.instpath = instpath;
        this.username = username;
        this.password = password;
        this.parameterFmt.parameterFmt(command, hardwareArchitecture, this.scantype, this.scanpath,
                this.compiler, this.cmd, this.targetos, this.extendedparameter);
        this.command = this.parameterFmt.toString();
        this.targetkernel = targetkernel;
        this.serviceType = serviceType;
        this.service = service;
        this.webserverPort = webserverPort;
        this.webserverIp = webserverIp;
        this.compilecommand = compilecommand;
        this.osMappingDir = osMappingDir;
        this.gllvm = gllvm;
        this.autoFix = autoFix;
        this.compilerType = compilerType;
        this.enableSVE = enableSVE;
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

    public String getCommand() {
        return command;
    }

    public String getUsername() {
        return username;
    }

    public String getEnableSVE() {
        return enableSVE;
    }

    public String getCompilerType() {
        return compilerType;
    }

    public String getAutoFix() {
        return autoFix;
    }

    public String getGllvm() {
        return gllvm;
    }

    public String getOsMappingDir() {
        return osMappingDir;
    }

    public String getCompilecommand() {
        return compilecommand;
    }

    public String getInstpath() {
        return instpath;
    }

    public Boolean getSourceenhancecheck() {
        return sourceenhancecheck;
    }

    public Boolean getInterpreted() {
        return interpreted;
    }

    public String getCustomizedmacros() {
        return customizedmacros;
    }

    public String getGfortran() {
        return gfortran;
    }

    public String getConstructtool() {
        return constructtool;
    }

    public String getWebserverPort() {
        return webserverPort;
    }

    public String getWebserverIp() {
        return webserverIp;
    }

    public String getServicetype() {
        return serviceType;
    }

    public String getTargetkernel() {
        return targetkernel;
    }
    public String getPassword() {
        return password;
    }
    @Override
    public void perform(@NonNull Run<?, ?> run, @NonNull FilePath workspace, @NonNull EnvVars env, Launcher launcher,
                        @NonNull TaskListener listener) throws InterruptedException, IOException {
        if (this.serviceType.equals("CLI")) {
            listener.getLogger().println("[WORKSPACE]" + workspace);
            VirtualChannel channel = launcher.getChannel();
            assert channel != null;
            new CopyToSlaveBuildWrapper().copyToSlave(run, launcher, listener, this.prefix);
            channel.call(new CommandMasterToSlaveCallable(this.command, listener, workspace.getParent(), this.prefix));
        } else if (this.serviceType.equals("webserver")) {
            try {
                switch (this.service) {
                    case "binaryCheck":
                        this.binaryTaskWebServer(run, listener, workspace);
                        break;
                    case "sourceCheck":
                        this.sourceCodeMigrationWebServer(run, listener, workspace);
                        break;
                    case "running_mode":
                        this.affinityAnalysisWebServer(run, listener, workspace, SrvType.running_mode);
                        break;
                    case "byte_alignment":
                        this.affinityAnalysisWebServer(run, listener, workspace, SrvType.byte_alignment);
                        break;
                    case "cacheline_alignment":
                        this.affinityAnalysisWebServer(run, listener, workspace, SrvType.cacheline_alignment);
                        break;
                    case "acceleration":
                        this.affinityAnalysisWebServer(run, listener, workspace, SrvType.acceleration);
                        break;
                    case "weakconsistency":
                        this.affinityAnalysisWebServer(run, listener, workspace, SrvType.weakconsistency);
                        break;
                    case "vecanalyzer":
                        this.affinityAnalysisWebServer(run, listener, workspace, SrvType.vecanalyzer);
                        break;
                    default:
                        break;
                }
            } catch (Exception e ) {
                run.setResult(Result.FAILURE);
                throw new RuntimeException(e);
            }
        }

    }

    private void binaryTaskWebServer(Run<?,?>  build, TaskListener listener, FilePath workspace) throws Exception {
        String serverIp = this.webserverIp != null ? this.webserverIp : "10.247.74.201";
        String serverPort = this.webserverPort != null ? this.webserverPort : "8087";
        PortadvBinaryProcess portadvBinaryProcess = new PortadvBinaryProcess(build, listener,
                workspace, serverIp, serverPort);
        portadvBinaryProcess.setPackpath(this.scanpath);
        portadvBinaryProcess.setInstpath(this.instpath);
        portadvBinaryProcess.setTargetos(this.targetos);
        portadvBinaryProcess.setTargetkernel(this.targetkernel);
        portadvBinaryProcess.setUsername(this.username);
        portadvBinaryProcess.setPassword(this.password);
        portadvBinaryProcess.process();

    }

    private void affinityAnalysisWebServer(Run<?,?> build, TaskListener listener, FilePath workspace, SrvType type) throws  Exception {
        String serverIp = this.webserverIp != null ? this.webserverIp : "10.247.74.201";
        String serverPort = this.webserverPort != null ? this.webserverPort : "8087";
        PortadvAffinityAnalysis obj = new PortadvAffinityAnalysis(username, password, build, listener, serverIp, serverPort, type, workspace);
        obj.setScanfile(this.copyAndGetScanPath());
        obj.setConstructtool(this.constructtool);
        obj.setCompilecommand(this.compilecommand);
        obj.setOsMappingDir(this.osMappingDir);
        obj.setGllvm(this.gllvm);
        obj.setTargetos(this.targetos);
        obj.setAutoFix(this.autoFix);
        obj.setCompilerType(this.compilerType);
        obj.setEnableSVE(this.enableSVE);
        obj.setUsername(this.username);
        obj.setPassword(this.password);
        obj.process();
    }

    private void sourceCodeMigrationWebServer(Run<?,?>  build, TaskListener listener, FilePath workspace) throws Exception {
        String serverIp = this.webserverIp != null ? this.webserverIp : "10.247.74.201";
        String serverPort = this.webserverPort != null ? this.webserverPort : "8087";
        SourceCodeMigration sourceCodeMigration = new SourceCodeMigration(build, serverIp, serverPort, listener, workspace);
        sourceCodeMigration.setUsername(this.username);
        sourceCodeMigration.setPassword(this.password);
        JSONObject processInfo = sourceCodeMigration.sourceCodeMigrationProcess(sourceCodeMigration, this.scanpath,
                this.constructtool, this.cmd, this.targetos, this.gfortran, this.customizedmacros,
                this.interpreted, this.sourceenhancecheck);
        if (processInfo.getBoolean("isFinish")) {
            sourceCodeMigration.downloadReport(processInfo.getString("taskID"), "2");
        }
    }

    /**
     * Web 服务的软件包扫描调用， 在perform中调用即可。1、设置API请求Data， 2、 调用process
     *
     * @throws Exception exception
     */

private String copyAndGetScanPath() {
        String userPath = this.scanpath;
        //遗留问题：添加拷贝逻辑，然后刷新真正的扫描路径 myscanpath
    String myscanpath = userPath;
    return myscanpath;
}

    private class CommandMasterToSlaveCallable extends jenkins.security.MasterToSlaveCallable<Void,IOException> {
        private final String prefix;
        private final String command;
        private final TaskListener listener;
        private final FilePath workspace;

        CommandMasterToSlaveCallable(String command, TaskListener listener,
                                     FilePath workspace, String prefix) {
            this.command =command;
            this.listener = listener;
            this.prefix = prefix;
            this.workspace = workspace;
        }



        @Override
        public Void call() throws IOException {
            try {
                // 在执行命令前，判断文件是否存在，不存在则进行copy处理
                listener.getLogger().println("[install] Installing package ...");
                PortingAdvisorInstall PAInstall = new PortingAdvisorInstall();
                FilePath installTarget;
                if (this.prefix== null || this.prefix.equals("")) {
                    listener.getLogger().println("[INFO] Prefix is empty. will be installed in the /opt");
                }
                installTarget = this.prefix != null ? new FilePath(new File(this.prefix)) : workspace;
                listener.getLogger().println("[workspace] is " +  workspace);
                listener.getLogger().println("installTarget is: " + installTarget);
                PAInstall.install(listener, parameterFmt.getHardwareArchitectures(), workspace, installTarget);
                BashCmdExec.callProcessBuilder(command, listener);
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
