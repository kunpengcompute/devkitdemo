package io.jenkins.plugins.devkit.slavecopy;

import hudson.EnvVars;
import hudson.Extension;
import hudson.FilePath;
import hudson.Launcher;
import hudson.model.*;
import hudson.remoting.VirtualChannel;
import hudson.tasks.BuildWrapper;
import hudson.tasks.BuildWrapperDescriptor;
import jenkins.model.Jenkins;

import org.jenkinsci.Symbol;
import org.jenkinsci.remoting.RoleChecker;
import org.jvnet.localizer.Localizable;
import org.jvnet.localizer.ResourceBundleHolder;
import org.kohsuke.stapler.DataBoundConstructor;

import java.io.File;
import java.io.IOException;
import java.util.StringTokenizer;

/**
 * The CopyToSlaveBuildSrapper
 *
 * @author d00345476
 * @version [dependencyAnalysis 1.0, 2023/3/13]
 * @since 2023/3/13
 */
public class CopyToSlaveBuildWrapper extends BuildWrapper {
    private FilePath prefix;
    private String includes;

    private boolean flatten;

    private String packageName = "Porting-advisor_23.0.RC1_linux-Kunpeng.tar.gz";
    @DataBoundConstructor
    public CopyToSlaveBuildWrapper() {
        this.includes = packageName;
        this.flatten = flatten;
        this.prefix = prefix;
    }

    @Override
    public Environment setUp(AbstractBuild build, final Launcher launcher, BuildListener listener)
        throws IOException, InterruptedException {
        String files = "Porting-advisor_23.0.RC1_linux-Kunpeng.tar.gz,Porting-advisor_23.0.RC1_linux-x86-64.tar.gz";
        StringTokenizer tokens = new StringTokenizer(files, ",");
        EnvVars env = build.getEnvironment(listener);
        env.overrideAll(build.getBuildVariables());
        FilePath projectWorkspaceOnSlave = build.getWorkspace().getParent();
        listener.getLogger().println("projectWorkspaceOnSlave is " + projectWorkspaceOnSlave);
        if (Computer.currentComputer() instanceof Jenkins.MasterComputer) {
            //主节点，从prefix 拷贝至workspace进行安装
            while (tokens.hasMoreTokens()) {
                String token = tokens.nextToken().trim();
//                listener.getLogger().println("token is " + token);
                String packagePath = this.prefix.toString() + File.separator + token;
//                listener.getLogger().println("packagePath is " + packagePath);
                if (new File(packagePath).exists()) {
                    listener.getLogger().println("[copy-to-slave] Trying to copy files: " + token + " from " + prefix +
                            " on the master to the  workspace :" + projectWorkspaceOnSlave );
                    MyFilePath.copyTo(this.prefix, token, flatten, projectWorkspaceOnSlave);
                } else {
                    listener.getLogger().println("[error]" + token + "] Not exist on " + prefix);
//                    build.setResult(Result.FAILURE);
                }
            }
            return null;
        } else {
            while (tokens.hasMoreTokens()) {
                String token = tokens.nextToken().trim();
                // 判断文件是否存在
                String path= projectWorkspaceOnSlave.getRemote() + File.separator + token;
                boolean isExistOnSlave = checkSlaveFile(prefix, path);
                if (!isExistOnSlave) {
                    // 不存在，则从master上拷贝
                    listener.getLogger().println("[COPY] Copy " + token + " to " + projectWorkspaceOnSlave);
                    MyFilePath.copyTo(prefix, token, flatten, projectWorkspaceOnSlave);
                } else {
                    listener.getLogger().println("[" + token + "] exist . [" + path + "]");
                }
            }
        }

        return new Environment() {
            @Override
            public boolean tearDown(AbstractBuild build, BuildListener listener)
                throws  IOException, InterruptedException {
                return true;
            }
        };
    }

    private boolean checkSlaveFile(FilePath source, String target) throws IOException, InterruptedException {
        return new FilePath.FileCallable<Boolean>() {
            @Override
            public void checkRoles(RoleChecker checker) throws SecurityException {
            }

            @Override
            public Boolean invoke(File f, VirtualChannel channel) throws IOException, InterruptedException {
                return new File(target).exists();
            }
        }.invoke(new File(source.getRemote()), FilePath.localChannel);
    }

    public Environment copyToSlave(Run<?,?> build, Launcher launcher, TaskListener listener, FilePath prefix)
            throws IOException, InterruptedException {
        this.prefix = prefix;
        AbstractBuild currentBuild = (AbstractBuild) build;
//        listener.getLogger().println("start to setup");
        return setUp(currentBuild, launcher, (BuildListener) listener);
    }

    @Override
    public DescriptorImpl getDescriptor() {
        return (DescriptorImpl) super.getDescriptor();
        }

    @Symbol("copy2slave")
    @Extension
    public static class DescriptorImpl extends BuildWrapperDescriptor {
        @Override
        public java.lang.String getDisplayName() {
            return new Localizable(ResourceBundleHolder.get(CopyToSlaveBuildWrapper.class), "DisplayName").toString();
        }
        @Override
        public boolean isApplicable(AbstractProject<?, ?> item) {
            return true;
        }
    }

}
