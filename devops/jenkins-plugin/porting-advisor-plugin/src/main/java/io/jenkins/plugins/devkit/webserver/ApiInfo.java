package io.jenkins.plugins.devkit.webserver;

public class ApiInfo {
    public static String getApiHead() {
        return "";
    }
    public static String getApiRootUrl(String webserverIp, String webserverPort) {
        return "https://" + webserverIp + ":" + webserverPort + "/framework/api/v1.0";
    }

    public static String getLoginUrl(String webserverIp, String webserverPort) {
        return "https://" + webserverIp + ":" + webserverPort + "/framework/api/v1.0/users/session/";
    }
    public static String getApiBinaryUrl(String webserverIp, String webserverPort, String apiType) {
        String apiHost = "https://" + webserverIp + ":" + webserverPort;
        switch (apiType) {
            case "binaryScan":
                apiHost += "/plugin/api/v1.0/porting/binary_scan/binary_scan_task/";
                break;
            case "scanProcess":
                apiHost += "/plugin/api/v1.0/porting/progress/";
                break;
            case "download_binaryScan":
                apiHost += "/plugin/api/v1.0/porting/binary_scan/download_binary_task/";
                break;
            case "totalTaskList":
                apiHost += "/framework/api/v1.0/taskmanage/total_tasks/";
                break;
            default:
                break;
        }
        return apiHost;
    }

    public static String getApiAffinityAnaUrl(String webserverIp, String webserverPort, SrvType srvType,
                                              String createOrGet) {
        String srvDesc = "";
        String resultDesc = "";
        switch (srvType) {
            case running_mode:
                srvDesc = srvType.name();
                break;
            case byte_alignment:
            case cacheline_alignment:
            case acceleration:
                srvDesc = srvType.name() + "/task";
                break;
            case weakconsistency:
            case vecanalyzer:
                srvDesc = srvType.name() + "/tasks";
                break;
            default:
                break;
        }
        if ( createOrGet.equals("create") ) {
            return "https://" + webserverIp + ":" + webserverPort + "/plugin/api/v1.0/affinity/" + srvDesc + "/";
        } else {
            return "https://" + webserverIp + ":" + webserverPort + "/plugin/api/v1.0/affinity/progress/";
        }
    }
    public static String getApiSourceCodeUrl(String webserverIp, String webserverPort, String apiType) {
        String apiHost = "https://" + webserverIp + ":" + webserverPort;
        switch (apiType) {
            case "sourceScan":
                apiHost += "/plugin/api/v1.0/porting/source_scan/";
                break;
            case "scanProcess":
                apiHost += "/plugin/api/v1.0/porting/progress/";
                break;
            case "taskList":
                apiHost += "/framework/api/v1.0/taskmanage/total_tasks/";
                break;
            default:
                break;
        }
        return apiHost;
    }


}
