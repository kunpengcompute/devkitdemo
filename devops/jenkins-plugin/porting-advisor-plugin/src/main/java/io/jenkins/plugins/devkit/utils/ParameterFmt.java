package io.jenkins.plugins.devkit.utils;

import org.apache.tools.ant.util.StringUtils;

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


    public void parameterFmt(String command, String hardwareArchitecture, String scantype, String scanpath,
                             String compiler, String cmd, String targetos, String extendedparameter) {
        this.setCommands(command);
        this.setHardwareArchitectures(hardwareArchitecture);
        this.setScantype(scantype);
        this.setScanpath(scanpath);
        this.setCompiler(compiler);
        this.setCmd(cmd);
        this.setTargetos(targetos);
        this.setExtendedparameter(extendedparameter);
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

    public void setExtendedparameter(String extendedparameter) {
        this.extendedparameter = extendedparameter;
    }

    @Override
    public String toString() {
        List<String> params = new ArrayList<String>();
        String toolHome = commands.split(" ")[0];
        params.add(toolHome);
        params.add(this.paramStrFmt(scantype, null));
        params.add(scanpath);
        if (compiler.length() > 0) {
            params.add(this.paramStrFmt("compiler", compiler));
        }
        if (cmd.length() > 0) {
            params.add(this.paramStrFmt("cmd", cmd));
        }
        params.add(this.paramStrFmt("targetos", targetos));
        if (extendedparameter.length() > 0) {
            params.add(extendedparameter);
        }
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
            case "cmd":
                fmtStr = "--cmd " + paramValue;
                break;
            case "compiler":
                fmtStr = "-C " + paramValue;
                break;
            case "output":
                fmtStr = "-O " + paramValue;
                break;
            case "targetos":
                fmtStr = "--tos " + paramValue;
                break;
            default:
                break;
        }
        return fmtStr;
    }

}
