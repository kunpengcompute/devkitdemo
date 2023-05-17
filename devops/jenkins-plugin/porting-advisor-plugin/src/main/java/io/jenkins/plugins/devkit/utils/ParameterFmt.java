package io.jenkins.plugins.devkit.utils;

import hudson.FilePath;
import hudson.model.TaskListener;
import org.apache.tools.ant.util.StringUtils;

import java.io.File;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

public class ParameterFmt implements Serializable {

    private String commands;
    private String hardwareArchitectures;
    private String scantype;
    private String scanpath;
    private String compiler;
    private String cmd;
    private String targetos;
    private String extendedparameter;
    private String prefix;

    public void ParameterFmt(String command, String hardwareArchitecture, String scantype, String scanpath,
                             String compiler, String cmd, String targetos, String extendedparameter, String prefix) {
        this.setCommands(command);
        this.setHardwareArchitectures(hardwareArchitecture);
        this.setScantype(scantype);
        this.setScanpath(scanpath);
        this.setCompiler(compiler);
        this.setCmd(cmd);
        this.setTargetos(targetos);
        this.setExtendedparameter(extendedparameter);
        this.setPrefix(prefix);
    }

    public String getPrefix() {
        return prefix;
    }

    private void setPrefix(String prefix) {
        this.prefix = prefix;
    }
    public String getCommands() {
        return commands;
    }
    private void setCommands(String commands) {
        this.commands = commands;
    }

    public String getHardwareArchitectures() {
        return hardwareArchitectures;
    }

    private void setHardwareArchitectures(String hardwareArchitectures) {
        this.hardwareArchitectures = hardwareArchitectures;
    }

    public String getScantype() {
        return scantype;
    }

    private void setScantype(String scantype) {
        this.scantype = scantype;
    }

    public String getScanpath() {
        return scanpath;
    }

    private void setScanpath(String scanpath) {
        this.scanpath = scanpath;
    }

    public String getCompiler() {
        return compiler;
    }

    private void setCompiler(String compiler) {
        this.compiler = compiler;
    }
    public String getCmd() {
        return cmd;
    }
    private void setCmd(String cmd) {
        this.cmd = cmd;
    }
    public String getTargetos() {
        return targetos;
    }

    private void setTargetos(String targetos) {
        this.targetos = targetos;
    }

    public String getExtendedparameter() {
        return extendedparameter;
    }

    private void setExtendedparameter(String extendedparameter) {
        this.extendedparameter = extendedparameter;
    }


    public String toStringCmd(TaskListener listener) {
        List<String> params = new ArrayList<String>();
//        listener.getLogger().println("[INFO]generate command");
        prefix = prefix.length() > 0 ? prefix : "/opt";
        String toolHome = prefix.trim() + File.separator + "portadv/tools/cmd/bin/porting-advisor";
//        listener.getLogger().println("[INFO]toolHome: "+ toolHome);
        params.add(toolHome);
        params.add(this.paramStrFmt(scantype, null));
//        listener.getLogger().println("[INFO]scantype: "+ scantype);
        params.add(scanpath);
//        listener.getLogger().println("[INFO]scanpath: "+ scanpath);
        if (compiler.length() > 0) {
            params.add(this.paramStrFmt("compiler", compiler));
        }
//        listener.getLogger().println("[INFO]compiler: "+ compiler);
        if (cmd.length() > 0) {
            params.add(this.paramStrFmt("cmd", cmd));
        }
//        listener.getLogger().println("[INFO]cmd: "+ cmd);
        params.add(this.paramStrFmt("targetos", targetos));
//        listener.getLogger().println("[INFO]targetos: "+ targetos);
        if (extendedparameter.length() > 0) {
            params.add(extendedparameter);
        }
//        listener.getLogger().println("[INFO]extendedparameter: "+ extendedparameter);
        return StringUtils.join(params, " ");
    }

    public String paramStrFmt(String paramKey, String paramValue) {
        String fmtStr = null;
        switch (paramKey) {
            case "sourcecode":
                fmtStr = "-S";
                break;
            case "binarypackage":
                fmtStr = "-P ";
                break;
            case "targetos":
                fmtStr = "--tos " + paramValue;
                break;
            case "compiler":
                fmtStr = "-C " + paramValue;
                break;
            case "cmd":
                fmtStr = "--cmd " + paramValue;
                break;
        }
        return fmtStr;
    }

}
