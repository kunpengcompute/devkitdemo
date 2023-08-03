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
import org.jenkinsci.plugins.workflow.job.WorkflowRun;
import org.jenkinsci.plugins.workflow.flow.FlowExecution;
import org.jenkinsci.plugins.workflow.actions.WorkspaceAction;
import org.jenkinsci.plugins.workflow.graph.FlowGraphWalker;
import org.jenkinsci.plugins.workflow.graph.FlowNode;
import org.jenkinsci.plugins.workflow.cps.nodes.StepStartNode;

import org.jenkinsci.remoting.RoleChecker;
import org.jvnet.localizer.Localizable;
import org.jvnet.localizer.ResourceBundleHolder;
import org.kohsuke.stapler.DataBoundConstructor;

import java.io.File;
import java.io.IOException;
import java.io.PrintStream;
import java.util.StringTokenizer;

/**
 * The CopyToSlaveBuildSrapper
 *
 * @author d00345476
 * @version [dependencyAnalysis 1.0, 2023/3/13]
 * @since 2023/3/13
 */
public class CopyToSlaveBuildWrapper extends BuildWrapper {
    private static String FPATH = "/opt/tmp";
    private String includes;

    private boolean flatten = false;

    private FilePath packageHomePath = new FilePath(new File(FPATH));
    private String packageName = "Porting-advisor_23.0.RC1_linux-Kunpeng.tar.gz";
    @DataBoundConstructor
    public CopyToSlaveBuildWrapper() {
        this.includes = packageName;
    }

    @Override
    public Environment setUp(AbstractBuild build, final Launcher launcher, BuildListener listener)
        throws IOException, InterruptedException {
        EnvVars env = build.getEnvironment(listener);
        env.overrideAll(build.getBuildVariables());
        if (Computer.currentComputer() instanceof Jenkins.MasterComputer) {
            //主节点，不copy
            listener.getLogger().println("No copy will take palce.");
            return null;
        } else {
            FilePath projectWorkspaceOnSlave = build.getWorkspace();
            if (projectWorkspaceOnSlave != null) {
                projectWorkspaceOnSlave = projectWorkspaceOnSlave.getParent();
            } else {
                listener.getLogger().println("projectWorkspaceOnSlave get null");
            }
            docopy(projectWorkspaceOnSlave, listener.getLogger());
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

    public Environment copyToSlave(Run<?,?> build, Launcher launcher, TaskListener listener, String prefix)
            throws IOException, InterruptedException {
        if (build instanceof  AbstractBuild) {
            AbstractBuild currentBuild = (AbstractBuild) build;
            return setUp(currentBuild, launcher, (BuildListener) listener);
        } else {
            FlowExecution exec = ((WorkflowRun)build).getExecution();
            if (exec == null) {
                listener.getLogger().println("getExecution get null");
            } else {
                FlowGraphWalker w = new FlowGraphWalker(exec);
                for (FlowNode n : w) {
                    if (n instanceof  StepStartNode) {
                        WorkspaceAction action = ((StepStartNode)n).getAction(WorkspaceAction.class);
                        if (action != null) {
                            FilePath projectWorkspaceOnSlave = new FilePath(new File(action.getPath()));
                            docopy(projectWorkspaceOnSlave, listener.getLogger());
                            break;
                        }
                    }
                }
            }
            return new Environment() {
                @Override
                public boolean tearDown(AbstractBuild build, BuildListener listener)
                    throws IOException, InterruptedException {
                    return true;
                }
            };
        }
    }

    private void docopy(FilePath projectWorkspaceOnSlave, PrintStream logger)
            throws IOException, InterruptedException {
        String files = "Porting-advisor_23.0.RC1_linux-Kunpeng.tar.gz,Porting-advisor_23.0.RC1_linux-x86-64.tar.gz";
        StringTokenizer tokens = new StringTokenizer(files, ",");
        while (tokens.hasMoreTokens()) {
            String token = tokens.nextToken().trim();
            //判断文件是否存在
            String path = projectWorkspaceOnSlave.getRemote() + File.separator + token;
            boolean isExistOnSlave = checkSlaveFile(packageHomePath, path);
            if (!isExistOnSlave) {
                //不存在，则从master上copy
                logger.println("[" + token + "] Not exist, Copying will begin.");
                logger.println("[COPY] " + packageHomePath + File.separator + token + " to " + projectWorkspaceOnSlave);
                MyFilePath.copyTo(packageHomePath, token, flatten, projectWorkspaceOnSlave);
            } else {
                logger.println("[" + token + " ] exist. [ " + path + "]");
            }
        }
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
