package io.jenkins.plugins.devkit;

import hudson.FilePath;
import hudson.model.TaskListener;
import io.jenkins.plugins.devkit.utils.BashCmdExec;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class PortingAdvisorInstall{
    private static String installPath = "/opt/portadv/tools/cmd/bin/porting-advisor";
    private static final String x86Package = "Porting-advisor_23.0.RC1_linux-x86-64.tar.gz";
    private static final String armPackage = "Porting-advisor_23.0.RC1_linux-Kunpeng.tar.gz";
    private static final String pluginPackage = "porting-advisor-plugin.jar";

    private static final String x86Url = "";
    private static final String armUrl = "";

    private void downloadPackage(String url, String outPath) {
    }

    private String getX86PackageDir() {
        return x86Package.split(".tar")[0];
    }

    private String getArmPackageDir() {
        return armPackage.split(".tar")[0];
    }

    private void getJenkinsPluginPackage(TaskListener listener) {
        List<String> cmds = new ArrayList<>();
        try {
            String jenkinsClassPath = this.getClass().getClassLoader().getResource("").getPath();
            listener.getLogger().println("jenkinsClassPath is: " + jenkinsClassPath);

            String packagePath = "../../../plugins/porting-advisor-plugin/WEB-INF/lib/" + pluginPackage;
            cmds.add("mkdir -p /opt/tmp/install ; cd " + jenkinsClassPath + " ; cp -r " + packagePath + "/opt/tmp/install");
            cmds.add("cd /opt/tmp/install ; unzip " + pluginPackage);

            for (String cmd : cmds) {
                BashCmdExec.callProcessBuilder(cmd, listener);
                listener.getLogger().println("cmd: " + cmd);
            }
        } catch (NullPointerException e) {
            e.printStackTrace();
            listener.getLogger().println("[ERROR]: " + e);
        }
    }

    private String getInstallCommand(String hardwareArchitecture, FilePath workspace, FilePath prefix) {
        String packageName = hardwareArchitecture.equals("x86") ? x86Package : armPackage;
        String packageDir = hardwareArchitecture.equals("x86") ? this.getX86PackageDir() : this.getArmPackageDir();

        return "mkdir -p " + prefix + File.separator + "/install; cd " + workspace + ";" +
                " tar --no-same-owner -zxvf " + packageName + " -C " + prefix + File.separator + "install; " +
                "cd " + prefix + File.separator + "install" + File.separator + packageDir + "; " +
                "echo Y | bash runtime_env_check.sh;" +
                "echo " + prefix + " | ./install cmd";
    }

    private boolean isInstalled() {
        File file = new File(installPath);
        return file.exists();
    }

    private boolean exists(String path) {
        return new File(path).exists();
    }

    public void install(TaskListener listener,String hardwareArchitecture, FilePath workspace, FilePath prefix)
        throws IOException, InterruptedException {
        String packageName = hardwareArchitecture.equals("x86") ? x86Package : armPackage;

        File file = new File(prefix + File.separator + "portadv/tools/cmd/bin/porting-advisor");
        if (file.exists()) {
            listener.getLogger().println("[INFO] The " + packageName + " has installed");
        } else {
            listener.getLogger().println("[start install]PortingAdvisor start install");
            BashCmdExec.callProcessBuilder(this.getInstallCommand(hardwareArchitecture, workspace, prefix), listener);
            BashCmdExec.callProcessBuilder("rm -rf " + prefix + "/install", listener);
        }
    }
}