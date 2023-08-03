package io.jenkins.plugins.devkit.webserver;

import java.io.*;
import hudson.FilePath;
import hudson.model.Result;
import hudson.model.Run;
import hudson.model.TaskListener;
import jnr.ffi.annotations.In;
import net.sf.json.JSONObject;

import java.io.PrintStream;
import java.net.HttpURLConnection;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Map;

public class PortadvBinaryProcess {
    private final String webserverIp;
    private final String webserverPort;
    private String packpath;
    private String instpath;
    private String targetos;
    private String targetkernel;
    private String apiUrl;
    private final Run<?, ?> build;
    private final TaskListener listener;
    private final FilePath workspace;
    private final PrintStream logger;
    private String username;
    private String password;

    public PortadvBinaryProcess(Run<?, ?> build, TaskListener listener, FilePath workspace,
                                String webserverIp, String webserverPort) {
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

    }
    public void setPackpath(String packpath) {
        this.packpath = packpath;
    }

    public void setInstpath(String instpath) {
        this.instpath = instpath;
    }

    public void setTargetos(String targetos) {
        this.targetos = targetos;
    }

    public void setTargetkernel(String targetkernel) {
        this.targetkernel = targetkernel;
    }

    public void setUsername(String username) {
        this.username = username;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    private String getData() {
        JSONObject params = new JSONObject();
        params.put("package_path", this.packpath);
        params.put("install_path", this.instpath);
        params.put("target_os", this.targetos);
        params.put("target_kernel", this.targetkernel);
        return params.toString();
    }
    private String createTask() throws Exception {
        //login and get the return token
        UserAccount userAccount = new UserAccount(this.username, this.password);
        String head = userAccount.getToken(this.webserverIp, this.webserverPort);
        this.apiUrl = ApiInfo.getApiBinaryUrl(this.webserverIp, this.webserverPort, "binaryScan");
        String taskID = "";
        BufferedReader in = null;
        try {
            HttpURLConnection connection = APIClient.httpResponse(this.apiUrl, head, this.getData(), "POST");
            int responseCode = connection.getResponseCode();
            in = new BufferedReader(new InputStreamReader(connection.getInputStream(), StandardCharsets.UTF_8));
            String inputLine;
            String resp;
            StringBuilder response = new StringBuilder();
            while ((inputLine = in.readLine()) != null) {
                response.append(inputLine);
            }
            byte[] bresult = response.toString().getBytes(StandardCharsets.UTF_8);
            resp = new String(bresult, StandardCharsets.UTF_8);
            listener.getLogger().println("Create data : " + resp);
            JSONObject jsonObject = JSONObject.fromObject(resp);
            Boolean checkStatus = APIClient.ApiErrCode((String) jsonObject.get("status"));
            listener.getLogger().println("Create result: " + jsonObject.get("infochinese"));
            if (checkStatus) {
                JSONObject data = (JSONObject) jsonObject.get("data");
                taskID = (String) data.get("task_id");
                listener.getLogger().println("扫描任务ID： " + taskID);
            } else {
                build.setResult(Result.FAILURE);
            }
            if (responseCode == 404) { listener.getLogger().println("在URL中的资源对象不存在"); }
            if (responseCode == 500) { listener.getLogger().println("服务内部错误"); }
        } catch (Exception e) {
            listener.getLogger().println("GET请求异常！" + e);
            e.printStackTrace();
            build.setResult(Result.FAILURE);
        } finally {
            try {
                if (in != null) {
                    in.close();
                }
            } catch (Exception e) {
                e.printStackTrace();
                build.setResult(Result.FAILURE);
            }
        }

        return taskID;
    }
    private Integer getTaskProcess(String taskID, String taskType) throws Exception {
        UserAccount userAccount = new UserAccount(this.username, this.password);
        String head = userAccount.getToken(this.webserverIp, this.webserverPort);
        this.apiUrl = ApiInfo.getApiBinaryUrl(this.webserverIp, this.webserverPort, "scanProcess");
        this.apiUrl += "?task_type=" + taskType + "&task_id=" + taskID;
        BufferedReader in = null;
        Integer progress = 0;
        try {
            HttpURLConnection connection = APIClient.httpResponse(this.apiUrl, head, "", "GET");
            StringBuilder response = new StringBuilder();
            in = new BufferedReader(new InputStreamReader(connection.getInputStream(), StandardCharsets.UTF_8));
            String inputLine;
            String resp;
            while ((inputLine = in.readLine()) != null) {
                response.append(inputLine);
            }
            byte[] bresult = response.toString().getBytes(StandardCharsets.UTF_8);
            resp = new String(bresult, StandardCharsets.UTF_8);
            JSONObject jsonObject = JSONObject.fromObject(resp);
            listener.getLogger().println("Task status: " + jsonObject.get("infochinese"));
            Boolean checkStatus = APIClient.ApiErrCode((String) jsonObject.get("status"));

            if (checkStatus) {
                progress = (Integer) ((JSONObject) jsonObject.get("data")).get("progress");
            } else {
                progress = -1;
                build.setResult(Result.FAILURE);
            }
        } catch (Exception e){
            listener.getLogger().println("GET请求异常！" + e);
            e.printStackTrace();
            build.setResult(Result.FAILURE);
        } finally {
            try {
                if (in != null) {
                    in.close();
                }
            } catch (Exception e) {
                e.printStackTrace();
                build.setResult(Result.FAILURE);
            }
        }

        return progress;
    }
    private boolean isTaskFinished(String taskID) throws Exception {
        Integer taskProgress = 0;
        while (true) {
            taskProgress = this.getTaskProcess(taskID, "7");
            if (taskProgress.equals(-1)) { return false; }
            String printProcess = new String(new char[taskProgress]).replace("\0", " |");
            listener.getLogger().println("【扫描进度】软件包扫描任务 ： " + taskID + "任务进度 " + taskProgress + "%" + printProcess);
            if (taskProgress.equals(100)) {
                return true;
            }
            Thread.sleep(300);
        }
    }
    public void downloadReport(String taskID, String reportType) throws Exception {
        BufferedReader in = null;
        UserAccount userAccount = new UserAccount(this.username, this.password);
        String head = userAccount.getToken(this.webserverIp, this.webserverPort);
        this.apiUrl = ApiInfo.getApiBinaryUrl(this.webserverIp, this.webserverPort, "download_binaryScan");
        this.apiUrl = this.apiUrl + "?report_type=" + reportType + "&task_id=" + taskID;
//        listener.getLogger().println("apiurl ：" + this.apiUrl);
        try {
            HttpURLConnection connection = APIClient.httpResponse(this.apiUrl, head, "", "GET");
            int responseCode = connection.getResponseCode();
            in = new BufferedReader(new InputStreamReader(connection.getInputStream(), StandardCharsets.UTF_8));
            String inputLine;
            StringBuilder response = new StringBuilder();
            while ((inputLine = in.readLine()) != null) {
                response.append(inputLine);
            }
            if (responseCode == 200) {
                String reportFile = this.workspace + File.separator + taskID + "_report.html";
                BufferedWriter out = new BufferedWriter(new OutputStreamWriter(Files.newOutputStream(Paths.get(reportFile)), StandardCharsets.UTF_8));
                out.write(String.valueOf(response));
                out.close();
                listener.getLogger().println("本地扫描报告路径： " + reportFile);
            } else {
                listener.getLogger().println("下载报告异常。");
                build.setResult(Result.FAILURE);
            }
        } catch (IOException e) {
            build.setResult(Result.FAILURE);
            e.printStackTrace();
        } finally {
            try {
                if (in != null) {
                    in.close();
                }
            } catch (Exception e) {
                e.printStackTrace();
                build.setResult(Result.FAILURE);
            }
        }

    }
    public void process() throws Exception {
        String responseID = this.createTask();
        if (this.isTaskFinished(responseID)) {
            this.downloadReport(responseID,  "2");
        }
    }
    public void report(String responseID) {
        //遗留问题：获取报告地址
    }

}
