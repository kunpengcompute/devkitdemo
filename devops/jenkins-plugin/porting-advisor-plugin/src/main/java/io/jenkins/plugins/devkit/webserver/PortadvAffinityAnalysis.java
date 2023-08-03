package io.jenkins.plugins.devkit.webserver;

import hudson.FilePath;
import hudson.model.Api;
import hudson.model.Result;
import hudson.model.Run;
import hudson.model.TaskListener;
import io.jenkins.cli.shaded.org.apache.sshd.common.util.buffer.Buffer;
import jnr.ffi.annotations.In;
import net.sf.json.JSONObject;

import java.io.*;
import java.net.HttpURLConnection;
import java.nio.charset.StandardCharsets;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class PortadvAffinityAnalysis {

    private final SrvType srvType;
    private final String webserverIp;
    private final String webserverPort;
    private final PrintStream logger;
    private final TaskListener listener;
    private Run<?, ?> build;
    private String stageOneID;
    private int stage;
    private FilePath workspace;
    private String scanfile;
    private String constructtool;
    private String osMappingDir;
    private String compilecommand;
    private String gllvm;
    private String targetos;
    private String autoFix;
    private String bcFileList;
    private String logtaskid;
    private String compilerType;
    private String enableSVE;
    private String username;
    private String password;

    public PortadvAffinityAnalysis(String username, String password, Run<?, ?> build, TaskListener listener,
                                   String webserverIp, String webserverPort, SrvType srvType,
                                   FilePath workspace) {
        this.srvType = srvType;
        this.webserverIp = webserverIp;
        this.webserverPort = webserverPort;
        this.workspace = workspace;
        this.listener = listener;
        if (listener != null) {
            this.logger = this.listener.getLogger();
        } else {
            this.logger = System.out;
        }
        this.build = build;
        this.stage = 1;
        this.stageOneID = "";
        this.logtaskid = "";

    }

    public void setScanfile(String scanfile) {
        this.scanfile = scanfile;
    }

    public void setConstructtool(String constructtool) {
        this.constructtool = constructtool;
    }

    public void setCompilecommand(String compilecommand) {
        this.compilecommand = compilecommand;
    }

    public void setGllvm(String gllvm) {
        this.gllvm = gllvm;
    }

    public void setTargetos(String targetos) {
        this.targetos = targetos;
    }

    public void setAutoFix(String autoFix) {
        this.autoFix = autoFix;
    }

    public void setCompilerType(String compilerType) {
        this.compilerType = compilerType;
    }

    public void setEnableSVE(String enableSVE) {
        this.enableSVE = enableSVE;
    }

    public void setUsername(String username) {
        this.username = username;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    public void setOsMappingDir(String osMappingDir) {
        this.osMappingDir = osMappingDir;
    }
    private void setResult(Result result) {
        if (this.build != null) {
            this.build.setResult(result);
        }
    }

    private String getData(SrvType srvType) {
        switch (srvType){
            case running_mode:
                return "{" +
                        "\"scan_file\": \"" + this.scanfile + "\", " +
                        "\"os_mapping_dir\": \"" + this.osMappingDir + "\"" +
                        "}";
            case byte_alignment:
            case acceleration:
                return "{\"info\":{" +
                        "\"sourcedir\": \"" + this.scanfile + "\", " +
                        "\"constructtool\": \"" + this.constructtool + "\", " +
                        "\"compilecommand\": \"" + this.compilecommand + "\"" +
                        "}}";
            case cacheline_alignment:
                return "{\"scan_file\": \"" + this.scanfile + "\"}";
            case weakconsistency:
                if (!this.scanfile.endsWith(".bc")) {
                    if (this.stage == 1) {
                        return "{" +
                                "\"sourcedir\": \"" + this.scanfile + "\", " +
                                "\"constructtool\": \"" + this.constructtool + "\", " +
                                "\"compilecommand\": \"" + this.compilecommand + "\", " +
                                "\"gllvm\": \"" + this.gllvm + "\", " +
                                "\"targetos\": \"" + this.targetos + "\"" +
                                "}";
                    } else {
                        return "{" +
                                "\"sourcedir\": \"" + this.scanfile + "\", " +
                                "\"bcFileList\": " + this.bcFileList + ", " +
                                "\"gllvm\": \"" + this.gllvm + "\", " +
                                "\"bcGenerateTaskId\": \"" + this.stageOneID + "\", " +
                                "\"autoFix\":" + this.autoFix +
                                "}";
                    }
                } else {
                    return "{\"bc_file\": \"" + this.scanfile + "\"}";
                }
            case vecanalyzer:
                if (!this.scanfile.endsWith(".bc")) {
                    if (this.stage == 1) {
                        return "{" +
                                "\"sourcedir\": \"" + this.scanfile + "\", " +
                                "\"constructtool\": \"" + this.constructtool + "\", " +
                                "\"compilecommand\": \"" + this.compilecommand + "\", " +
                                "\"gllvm\": \"" + this.gllvm + "\", " +
                                "\"targetos\": \"" + this.targetos + "\"" +
                                "}";
                    } else {
                        return "{" +
                                "\"sourcedir\": \"" + this.scanfile + "\", " +
                                "\"bcFileList\": " + this.bcFileList + ", " +
                                "\"gllvm\": \"" + this.gllvm + "\", " +
                                "\"bcGenerateTaskId\": \"" + this.stageOneID + "\", " +
                                "\"autoFix\": " + this.autoFix + "," +
                                "\"compilerType\": \"" + this.compilerType + "\", " +
                                "\"enableSVE\": " + this.enableSVE + "," +
                                "\"logGenerateTaskId\": \"" + this.logtaskid + "\"" +
                                "}";
                    }
                } else {
                    return "{\"bc_file\": \"" + this.scanfile + "\"," +
                            "\"compilerType\": \"" + this.compilerType + "\", " +
                            "\"enableSVE\": " + this.enableSVE +
                            "}";
                }
            default:
                return "";
        }
    }
    private String getTaskType() {
        switch (this.srvType){
            case running_mode:
                return "5";
            case byte_alignment:
                return "6";
            case cacheline_alignment:
                return "12";
            case acceleration:
                return "13";
            case weakconsistency:
                if (!this.scanfile.endsWith(".bc")) {
                    if (this.stage == 1) {
                        return "9";
                    } else {
                        return "10";
                    }
                } else {
                    return "11";
                }
            case vecanalyzer:
                if (!this.scanfile.endsWith(".bc")) {
                    if (this.stage == 1) {
                        return "14";
                    } else if (this.stage == 2) {
                        return "17";
                    } else {
                        return "15";
                    }
                } else {
                    return "16";
                }
            default:
                return "";
        }
    }
    private String createTask() throws Exception {
        String api_url = ApiInfo.getApiAffinityAnaUrl(this.webserverIp, this.webserverPort,
                this.srvType, "create");
        if (this.srvType == SrvType.weakconsistency || this.srvType == SrvType.vecanalyzer) {
            if (!this.scanfile.endsWith(".bc") && this.stage == 1) {
                api_url += "compilefile/";
            }
        }
        //login and get the return token
        UserAccount userAccount = new UserAccount(this.username, this.password);
        String head = userAccount.getToken(this.webserverIp, this.webserverPort);
        String apiData = this.getData(srvType);
        this.logger.println("【创建扫描任务】开始 : " + api_url + apiData);
        HttpURLConnection connection = APIClient.httpResponse(api_url, head, apiData, "POST");
        int responseCode = connection.getResponseCode();
        this.logger.println(responseCode);
        String responseID = "";
        if (responseCode == 200) {
            BufferedReader in = new BufferedReader(new InputStreamReader(connection.getInputStream(), "UTF-8"));
            String inputLine;
            StringBuffer response = new StringBuffer();
            while ((inputLine = in.readLine()) != null) {
                response.append(inputLine);
                if (inputLine.contains("id\":\"")) {
                    responseID = inputLine.split("id\":\"")[1].split("\"")[0].trim();
                }
            }
            in.close();
            //打印响应结果
            this.logger.println("【创建扫描任务】Response : " + response);
            if (responseID.length() == 0) {
                this.logger.println("创建任务失败，没有生成任务ID");
                this.setResult(Result.FAILURE);
            }
        }
        if (responseCode == 400 || responseCode == 404) {
            this.logger.println("在URL中的资源对象不存在");
            this.setResult(Result.FAILURE);
        }
        if (responseCode == 500) {
            this.logger.println("服务内部错误");
            this.setResult(Result.FAILURE);
        }
        return responseID;
    }

    private Integer getTaskProcess(String taskID) throws Exception {
        UserAccount userAccount = new UserAccount(this.username, this.password);
        String head = userAccount.getToken(this.webserverIp, this.webserverPort);
        String apiData = "";
        String apiUrl = ApiInfo.getApiAffinityAnaUrl(this.webserverIp, this.webserverPort,
                this.srvType, "progress");
        String url = apiUrl + "?task_type=" + this.getTaskType() + "&task_id=" + taskID;
        this.logger.println(url);
        HttpURLConnection connection = APIClient.httpResponse(url, head, apiData, "GET");
        int responseCode = connection.getResponseCode();
        this.logger.println("Response Code : " + responseCode);
        StringBuffer response = new StringBuffer();
        if (responseCode == 200) {
            BufferedReader in = new BufferedReader(new InputStreamReader(connection.getInputStream(), "UTF-8"));
            String inputLine;
            while ((inputLine = in.readLine()) != null) {
                response.append(inputLine);
            }
            in.close();
        }
        this.logger.println("【查询任务进度】Response : " + response);
        Integer progress = -1;
        if (response.toString().contains("progress\":")) {
            progress = Integer.parseInt(response.toString().split("progress\":")[1].split(",")[0].trim());
        }
        if (response.toString().contains("0x0d0500") || response.toString().contains("0x0d0502")
                || response.toString().contains("0x0d0400") || response.toString().contains("0x0d0402")
                || response.toString().contains("0x0d0c00") || response.toString().contains("0x0d0c02")
                || response.toString().contains("0x0e0300") || response.toString().contains("0x0e0302")
                || response.toString().contains("0x0d0b00")
                || response.toString().contains("0x0d0a00")
                || response.toString().contains("0x0d0e00")
                || response.toString().contains("0x0d0d00") || response.toString().contains("0x0d0d02")) {
            if (progress < 0) {
                progress = 100;
            }
        } else if (response.toString().contains("0x0d0501")
                || response.toString().contains("0x0d0401")
                || response.toString().contains("0x0d0c01")
                || response.toString().contains("0x0e0301")
                || response.toString().contains("0x0d0b01")
                || response.toString().contains("0x0d0a01")
                || response.toString().contains("0x0d0e01")) {
            if (progress < 0) {
                progress = 10;
            }
        } else {
            if (this.srvType == SrvType.vecanalyzer && this.stage == 2) {
                progress =100;
            } else {
                System.out.println("任务执行失败");
                this.setResult(Result.FAILURE);
            }
        }
        return progress;
    }
    private boolean isTaskFinished(String taskID) throws Exception {
        Integer taskProgress = 0;
        while (true) {
            taskProgress = this.getTaskProcess(taskID);
            String printProcess = new String(new char[taskProgress]).replace("\0", " |");
            this.logger.println("【扫描进度】任务 ： " + taskID + ": 任务进度 " + taskProgress + "%" + printProcess);
            if (taskProgress.equals(100)) {
                return true;
            }
            Thread.sleep(3000);
        }

    }
    public void downloadReport(String responseID) throws Exception {
        UserAccount userAccount = new UserAccount(this.username, this.password);
        String head = userAccount.getToken(this.webserverIp, this.webserverPort);
        String apiData = "";
        String apiUrl = ApiInfo.getApiAffinityAnaUrl(this.webserverIp, this.webserverPort,
                this.srvType, "get");
        String url = apiUrl + "?task_type=" + this.getTaskType() + "&task_id=" + responseID + "&download_html=true";
        HttpURLConnection connection = APIClient.httpResponse(url, head, apiData, "GET");
        int responseCode = connection.getResponseCode();
        this.logger.println("Response Code : " + responseCode);
        if (responseCode == 200) {
            BufferedReader in = new BufferedReader(new InputStreamReader(connection.getInputStream(), "UTF-8"));
            String inputLine;
            StringBuffer response = new StringBuffer();
            while ((inputLine = in.readLine()) != null) {
                response.append(inputLine);
            }
            in.close();
            //打印响应结果
            this.logger.println("【下载报告成功】--->下载成功 ： " + response);
            String reportFile = this.workspace + File.separator + responseID + "_report.html";
            String buildJobUrl = "";
            if (this.build != null) {
                buildJobUrl = this.build.getUrl();
            }
            Pattern r = Pattern.compile("(\\d+)");
            Matcher m = r.matcher(buildJobUrl);
            String buildJobNum = "";
            if (m.find()) { buildJobNum = m.group(1); }
            String reportUrl = "http://" + webserverIp + ":" + webserverPort + "/" +buildJobUrl + "execution/node/" +
                    buildJobNum + "/ws/" + responseID + "_report.html";
            try {
                BufferedWriter out = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(reportFile), "UTF-8"));
                out.write(String.valueOf(response));
                out.close();
                this.logger.println("本地扫描报告路径： " + reportFile);
//                this.logger.println("在线扫描报告链接： " + reportUrl);
            } catch (IOException e) {
                this.setResult(Result.FAILURE);
                throw  e;
            }
        }
    }
    public String getBcFileList() throws Exception {
        UserAccount userAccount = new UserAccount(this.username, this.password);
        String head = userAccount.getToken(this.webserverIp, this.webserverPort);
        String apiData = "";
        String bcFileList = "";
        String apiUrl = ApiInfo.getApiAffinityAnaUrl(this.webserverIp, this.webserverPort,
                this.srvType, "create") + this.stageOneID + "/bcfilelist/";
        HttpURLConnection connection = APIClient.httpResponse(apiUrl, head, apiData, "GET");
        int responseCode = connection.getResponseCode();
        this.logger.println("Response Code : " + responseCode);
        if (responseCode == 200) {
            BufferedReader in = new BufferedReader(new InputStreamReader(connection.getInputStream(), "UTF-8"));
            String inputLine;
            StringBuffer response = new StringBuffer();
            while ((inputLine = in.readLine()) != null) {
                response.append(inputLine);
            }
            in.close();
            //打印响应结果
            this.logger.println("【获取bcFileList】--->成功 : " + response);
            if (response.toString().contains("bc_file_list\":[")) {
                bcFileList = "[" + response.toString().split("bc_file_list\":\\[")[1].split("\\]")[0].trim() + "]";
            }
        } else {
            this.logger.println("[获取bcFileList失败]");
            this.setResult(Result.FAILURE);
        }
        return bcFileList;
    }
    public String getCompileLogID() throws Exception {
        UserAccount userAccount = new UserAccount(this.username, this.password);
        String head = userAccount.getToken(this.webserverIp, this.webserverPort);
        this.logtaskid = "";
        String apiData = this.getData(srvType);
        String apiUrl = ApiInfo.getApiAffinityAnaUrl(this.webserverIp, this.webserverPort,
                this.srvType, "create") + "/compilelog/";
        HttpURLConnection connection = APIClient.httpResponse(apiUrl, head, apiData, "POST");
        int responseCode = connection.getResponseCode();
        this.logger.println("Response Code : " + responseCode);
        if (responseCode == 200) {
            BufferedReader in = new BufferedReader(new InputStreamReader(connection.getInputStream(), "UTF-8"));
            String inputLine;
            StringBuffer response = new StringBuffer();
            while ((inputLine = in.readLine()) != null) {
                response.append(inputLine);
            }
            in.close();
            //打印响应结果
            this.logger.println("【getCompileLogID】-->成功 ： " + response);
            if (response.toString().contains("log_task_id\":\"")) {
                logtaskid = response.toString().split("log_task_id\":\"")[1].split("\"")[0].trim();
            }
        } else {
            this.logger.println("[getCompileLogID失败]");
            this.setResult(Result.FAILURE);
        }
        return logtaskid;


    }

    public void process() throws Exception {
        if (this.srvType == SrvType.weakconsistency || this.srvType == SrvType.vecanalyzer) {
            String responseID = this.createTask();
            if (this.isTaskFinished(responseID)) {
                this.stageOneID = responseID;
                this.bcFileList = getBcFileList();
                this.stage = 2;
                if (this.srvType == SrvType.vecanalyzer) {
                    this.logtaskid = getCompileLogID();
                    this.isTaskFinished(this.logtaskid);
                    this.stage = 3;
                }
            }
        }
        String responseID = this.createTask();
        if (this.isTaskFinished(responseID)) {
            this.downloadReport(responseID);
        }
    }
    public static void main(String[] args) throws Exception {

    }
}
