package io.jenkins.plugins.devkit.utils;

import net.sf.json.JSON;
import net.sf.json.JSONObject;
import net.sf.json.JSONSerializer;
import net.sf.json.JsonConfig;
import org.apache.commons.lang.StringUtils;

import java.io.Serializable;

public class SourceScanParamsFmt implements Serializable {
    private String sourcedir;
    private JSONObject compiler;
    private JSONObject cgocompiler;
    private String constructtool;
    private String compilecommand;
    private String targetos;
    private String gfortran;
    private String customizedmacros;
    private Boolean interpreted;
    private Boolean sourceenhancecheck;

    public void sourceScanParamsFmt(String sourcedir, String constructtool, String compilecommand, String targetos,
                                    String gfortran, String customizedmacros, Boolean interpreted,
                                    Boolean sourceenhancecheck) {
        this.setSourcedir(sourcedir);
        this.setConstructtool(constructtool);
        this.setCompilecommand(compilecommand);
        this.setTargetos(targetos);
        this.setGfortran(gfortran);
        this.setCustomizedmacros(customizedmacros);
        this.setInterpreted(interpreted);
        this.setSourceenhancecheck(sourceenhancecheck);
    }

    public void setConstructtool(String constructtool) {
        this.constructtool = constructtool;
    }

    public String getConstructtool() {
        return constructtool;
    }

    public void setSourcedir(String sourcedir) {
        this.sourcedir = sourcedir;
    }

    public String getSourcedir() {
        return sourcedir;
    }

    public void setCompilecommand(String compilecommand) {
        this.compilecommand = compilecommand;
    }

    public String getCompilecommand() {
        return compilecommand;
    }

    public void setTargetos(String targetos) {
        this.targetos = targetos;
    }

    public String getTargetos() {
        return targetos;
    }

    public void setGfortran(String gfortran) {
        this.gfortran = gfortran;
    }

    public String getGfortran() {
        return gfortran;
    }

    public void setCustomizedmacros(String customizedmacros) {
        this.customizedmacros = customizedmacros;
    }

    public String getCustomizedmacros() {
        return customizedmacros;
    }

    public void setInterpreted(Boolean interpreted) {
        this.interpreted = interpreted;
    }

    public Boolean getInterpreted() {
        return interpreted;
    }

    public void setSourceenhancecheck(Boolean sourceenhancecheck) {
        this.sourceenhancecheck = sourceenhancecheck;
    }

    public Boolean getSourceenhancecheck() {
        return sourceenhancecheck;
    }

    public JSONObject getCompiler() {
        JSONObject compilerOb = new JSONObject();
        JSONObject targetOSInfo = this.toKernel(this.getTargetos());
        compilerOb.put("type", targetOSInfo.get("compiler"));
        compilerOb.put("version", targetOSInfo.get("version"));
        return compilerOb;
    }

    public void setCompiler(JSONObject compiler) {
        this.compiler = compiler;
    }

    public JSONObject getCgocompiler() {
        JSONObject cgocompilerOb = new JSONObject();
        JSONObject targetOSInfo = this.toKernel(this.getTargetos());
        cgocompilerOb.put("type", targetOSInfo.get("compiler"));
        cgocompilerOb.put("version", targetOSInfo.get("version"));
        return cgocompilerOb;
    }

    public void setCgocompiler(JSONObject cgocompiler) {
        this.cgocompiler = cgocompiler;
    }

    private static JSON setNullCompilerOrCgocompiler() {
        return JSONSerializer.toJSON("{\"type\":\"\",\"version\":\"\"}", new JsonConfig());
    }

    public String createTask() {
        JSONObject params = new JSONObject();
        JSONObject targetOSInfo = this.toKernel(targetos);
        params.put("sourcedir", sourcedir);
        params.put("compilecommand", compilecommand);
        params.put("targetos", targetos);
        params.put("targetkernel", targetOSInfo.get("kernel"));
        params.put("interpreted", interpreted);
        params.put("sourceenhancecheck", sourceenhancecheck);
        if (interpreted.equals(Boolean.TRUE) && StringUtils.isBlank(compilecommand)) {
            params.put("compiler", setNullCompilerOrCgocompiler());
            params.put("cgocompiler", setNullCompilerOrCgocompiler());
            params.put("constructtool", "");
            params.put("gfortran", "");
            params.put("customizedmacros", "");
        } else {
            params.put("compiler", this.getCompiler());
            params.put("cgocompiler", this.getCgocompiler());
            params.put("constructtool", constructtool);
            params.put("gfortran", gfortran);
            params.put("customizedmacros", customizedmacros);
        }
        return "{\"info\": " + params + "}";
    }

    public JSONObject toKernel(String targetOS) {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put("centos7.4", "{\"kernel\": \"4.11.0\", \"compiler\": \"gcc\", \"version\": \"4.8.5\"}");
        jsonObject.put("centos7.5", "{\"kernel\": \"4.14.0\", \"compiler\": \"gcc\", \"version\": \"4.8.5\"}");
        jsonObject.put("centos7.6", "{\"kernel\": \"4.14.0\", \"compiler\": \"gcc\", \"version\": \"4.8.5\"}");
        jsonObject.put("centos7.7", "{\"kernel\": \"4.18.0\", \"compiler\": \"gcc\", \"version\": \"4.8.5\"}");
        jsonObject.put("centos8.0", "{\"kernel\": \"4.18.0\", \"compiler\": \"gcc\", \"version\": \"8.2.0\"}");
        jsonObject.put("centos8.1", "{\"kernel\": \"4.18.0\", \"compiler\": \"gcc\", \"version\": \"8.3.0\"}");
        jsonObject.put("centos8.2", "{\"kernel\": \"4.18.0\", \"compiler\": \"gcc\", \"version\": \"8.3.0\"}");
        jsonObject.put("neokylinv7u5", "{\"kernel\": \"4.14.0\",\"compiler\": \"gcc\", \"version\": \"4.8.5\"}");
        jsonObject.put("neokylinv7u6", "{\"kernel\": \"4.14.0\", \"compiler\": \"gcc\", \"version\": \"4.8.5\"}");
        jsonObject.put("deepinv15.2", "{\"kernel\": \"4.19.34\", \"compiler\": \"gcc\", \"version\": \"6.3.0\"}");
        jsonObject.put("ubuntu18.04.x", "{\"kernel\": \"4.15.0\", \"compiler\": \"gcc\", \"version\": \"7.3.0\"}");
        jsonObject.put("linxos6.0.90", "{\"kernel\": \"4.19.0\", \"compiler\": \"gcc\", \"version\": \"6.3.0\"}");
        jsonObject.put("debian10", "{\"kernel\": \"4.14.0\", \"compiler\": \"gcc\", \"version\": \"8.3.0\"}");
        jsonObject.put("euleros2.8", "{\"kernel\": \"4.19.36\", \"compiler\": \"gcc\", \"version\": \"7.3.0\"}");
        jsonObject.put("openeuler22.03", "{\"kernel\": \"5.10.0\", \"compiler\": \"bisheng compiler\", \"version\": \"2.5.0.1\"}");
        jsonObject.put("openeuler20.03", "{\"kernel\": \"4.19.90\", \"compiler\": \"bisheng compiler\", \"version\": \"2.5.0.1\"}");
        jsonObject.put("openeuler20.03sp1", "{\"kernel\": \"4.19.90\", \"compiler\": \"gcc\", \"version\": \"7.3.0\"}");
        jsonObject.put("openeuler20.03sp2", "{\"kernel\": \"4.19.90\", \"compiler\": \"gcc\", \"version\": \"7.3.0\"}");
        jsonObject.put("openeuler20.03sp3", "{\"kernel\": \"4.19.90\", \"compiler\": \"gcc\", \"version\": \"7.3.0\"}");
        jsonObject.put("openeuler22.03sp1", "{\"kernel\": \"5.10.0\", \"compiler\": \"gcc\", \"version\": \"10.3.0\"}");
        jsonObject.put("susesles15.1", "{\"kernel\": \"4.12.14\", \"compiler\": \"gcc\", \"version\": \"7.4.0\"}");
        jsonObject.put("uos20sp1", "{\"kernel\": \"4.19.0\", \"compiler\": \"gcc\", \"version\": \"8.3.0\"}");
        jsonObject.put("kylinv10sp1", "{\"kernel\": \"4.19.90\", \"compiler\": \"gcc\", \"version\": \"7.3.0\"}");
        jsonObject.put("kylinv10sp2", "{\"kernel\": \"4.19.90\", \"compiler\": \"gcc\", \"version\": \"7.3.0\"}");
        jsonObject.put("kylinv10sp3", "{\"kernel\": \"4.19.90\", \"compiler\": \"gcc\", \"version\": \"7.3.0\"}");
        jsonObject.put("kylinsecos3.4", "{\"kernel\": \"4.19.90\", \"compiler\": \"gcc\", \"version\": \"7.3.0\"}");
        jsonObject.put("ubuntu20.04.x", "{\"kernel\": \"5.4.0\", \"compiler\": \"gcc\", \"version\": \"9.3.0\"}");
        jsonObject.put("uoseuler20", "{\"kernel\": \"4.19.90\", \"compiler\": \"gcc\", \"version\": \"7.3.0\"}");
        jsonObject.put("uos20-1050e", "{\"kernel\": \"4.19.90\", \"compiler\": \"gcc\", \"version\": \"7.3.0\"}");
        jsonObject.put("uos20-1020e", "{\"kernel\": \"4.19.90\", \"compiler\": \"gcc\", \"version\": \"7.3.0\"}");
        jsonObject.put("bclinux7.6", "{\"kernel\": \"4.19.25\", \"compiler\": \"gcc\", \"version\": \"4.8.5\"}");
        jsonObject.put("bclinux7.7", "{\"kernel\": \"4.19.25\", \"compiler\": \"gcc\", \"version\": \"4.8.5\"}");
        jsonObject.put("isoft5.1", "{\"kernel\": \"4.19.90\", \"compiler\": \"gcc\", \"version\": \"7.3.0\"}");

        String kernelInfo = (String) jsonObject.get(targetOS);
        return JSONObject.fromObject(kernelInfo);
    }

}
