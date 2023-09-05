package io.jenkins.plugins.devkit.webserver;

import hudson.FilePath;
import hudson.model.Result;
import hudson.model.Run;
import hudson.model.TaskListener;
import io.jenkins.plugins.devkit.utils.HtmlRepoFiles;
import io.jenkins.plugins.devkit.utils.SourceScanParamsFmt;
import net.sf.json.JSONObject;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.nio.charset.StandardCharsets;

public class SourceCodeMigration {

    private final String webserverIp;
    private final String webserverPort;
    private final String createTaskRequestMethod;
    private final String getTaskRequestMethod;
    private String apiUrl;
    private Run<?, ?> build;
    private FilePath workspace;

    private TaskListener listener;

    private String username;
    private String password;

    SourceScanParamsFmt sourceScanParamsFmt = new SourceScanParamsFmt();
    public SourceCodeMigration(Run<?,?> build, String webserverIp,
                               String webserverPort, TaskListener listener, FilePath workspace) {
        this.webserverIp = webserverIp;
        this.webserverPort = webserverPort;
        this.createTaskRequestMethod = "POST";
        this.getTaskRequestMethod = "GET";
        this.listener = listener;
        this.workspace = workspace;
        this.build = build;

    }

    public String getUsername() {
        return username;
    }

    public String getPassword() {
        return password;
    }

    public void setUsername(String username) {
        this.username = username;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    private String createTask(String createTaskParam) {
        BufferedReader in = null;
        String taskID = "";
        this.apiUrl = ApiInfo.getApiSourceCodeUrl(this.webserverIp, this.webserverPort, "sourceScan");
        try {
            //login and get the return token
            UserAccount userAccount = new UserAccount(this.username, this.password);
            String head = userAccount.getToken(this.webserverIp, this.webserverPort);
            listener.getLogger().println("URL: " + this.apiUrl);
            listener.getLogger().println("Request Data: " + createTaskParam);
            HttpURLConnection connection = APIClient.httpResponse(this.apiUrl, head, createTaskParam, this.createTaskRequestMethod);
            int responseCode = connection.getResponseCode();
            listener.getLogger().println("Response Code: " + responseCode);
            in = new BufferedReader(new InputStreamReader(connection.getInputStream(), StandardCharsets.UTF_8));
            String line;
            StringBuilder response = new StringBuilder();
            String resp = "";
            while ((line = in.readLine()) != null) {
                response.append(line);
            }
            byte[] bresult = response.toString().getBytes(StandardCharsets.UTF_8);
            resp = new String(bresult, StandardCharsets.UTF_8);
            listener.getLogger().println("Create data: " + resp);
            JSONObject jsonObject = JSONObject.fromObject(resp);
            listener.getLogger().println("Create result: " + jsonObject.get("infochinese"));
            Boolean checkStatus = APIClient.ApiErrCode((String) jsonObject.get("status"));
            if (checkStatus) {
                JSONObject data = (JSONObject) jsonObject.get("data");
                taskID = (String) data.get("id");
                listener.getLogger().println("扫描任务ID： " + data.get("id"));
            } else {
                build.setResult(Result.FAILURE);
            }
        } catch (Exception e) {
            listener.getLogger().println("GET请求异常! " + e);
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

    public Integer getTaskProcess(String taskType, String taskID) {
        StringBuffer buf = new StringBuffer();
        String result = "";
        Integer progress = null;
        BufferedReader in = null;
        this.apiUrl = ApiInfo.getApiSourceCodeUrl(this.webserverIp, this.webserverPort, "scanProcess");
        this.apiUrl += "?task_type=" + taskType + "&task_id=" + taskID;
        try {
            //login and get the return token
            UserAccount userAccount = new UserAccount(this.username, this.password);
            String head = userAccount.getToken(this.webserverIp, this.webserverPort);
            HttpURLConnection connection = APIClient.httpResponse(this.apiUrl, head, "", this.getTaskRequestMethod);
            int responseCode = connection.getResponseCode();
            listener.getLogger().println("Response Code : " + responseCode);
            in = new BufferedReader(new InputStreamReader(connection.getInputStream(), StandardCharsets.UTF_8));
            String line;
            while ((line = in.readLine()) != null) {
                buf.append(line);
            }
            byte[] bresult = buf.toString().getBytes(StandardCharsets.UTF_8);
            result = new String(bresult, StandardCharsets.UTF_8);
            JSONObject jsonObject = JSONObject.fromObject(result);
            JSONObject resData = (JSONObject) jsonObject.get("data");
            progress = (Integer) resData.get("progress");
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
        return progress;
    }

    private JSONObject isTaskFinished(String taskType, String taskID) throws Exception {
        Integer taskProgress = 0;
        JSONObject processStatus = new JSONObject();
        processStatus.put("taskID", taskID);
        processStatus.put("taskType", taskType);
        while (true) {
            taskProgress = this.getTaskProcess(taskType, taskID);
            if (taskProgress.equals(-1)) {
                processStatus.put("isFinish", false);
                break;
            }
            String printProcess = new String(new char[taskProgress]).replace("\0", " |");
            listener.getLogger().println("[【迁移进度】源码迁移任务：" + taskID + "： 任务进度" + taskProgress + "%" + printProcess);
            if (taskProgress.equals(100)) {
                processStatus.put("isFinish", true);
                break;
            }
            Thread.sleep(300);
        }
        return processStatus;
    }

    public void downloadReport(String taskID, String reportType) throws Exception {
        UserAccount userAccount = new UserAccount(this.username, this.password);
        BufferedReader in = null;
        String head = userAccount.getToken(this.webserverIp, this.webserverPort);
        this.apiUrl = ApiInfo.getApiSourceCodeUrl(this.webserverIp, this.webserverPort, "sourceScan");
        this.apiUrl = this.apiUrl + taskID + "/download/?report_type=" + reportType;
        HttpURLConnection connection = APIClient.httpResponse(this.apiUrl, head, "", this.getTaskRequestMethod);
        int responseCode = connection.getResponseCode();
        listener.getLogger().println("response Code : " + responseCode);
        try {
            in = new BufferedReader(new InputStreamReader(connection.getInputStream(), StandardCharsets.UTF_8));
            String inputLine;
            StringBuilder response = new StringBuilder();
            while ((inputLine = in.readLine()) != null) {
                response.append(inputLine);
            }
            if (responseCode == 200) {
                HtmlRepoFiles.getInstance().writeTo(this.workspace, taskID, response.toString(), this.listener);
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

    public JSONObject sourceCodeMigrationProcess(SourceCodeMigration sourceCodeMigration, String sourcedir,
                                                 String constructtool, String compilecommand, String targetos,
                                                 String gfortran, String customizedmacros, Boolean interpreted,
                                                 Boolean sourceenhancecheck) throws Exception {
        String taskType = "0";
        String reportType = "1";
        sourceScanParamsFmt.sourceScanParamsFmt(sourcedir, constructtool, compilecommand, targetos, gfortran,
                customizedmacros, interpreted, sourceenhancecheck);
        String createTaskParam = sourceScanParamsFmt.createTask();
        String taskID = sourceCodeMigration.createTask(createTaskParam);
        return sourceCodeMigration.isTaskFinished(taskType, taskID);
    }

}
