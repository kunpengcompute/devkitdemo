package io.jenkins.plugins.devkit.webserver;

import net.sf.json.JSONObject;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.nio.charset.StandardCharsets;

public class UserAccount {
    private final String username;
    private final String password;

    public UserAccount(String username, String password) {
        this.username = username;
        this.password = password;
    }

    private String getData() {
        JSONObject params = new JSONObject();
        params.put("username", this.username);
        params.put("password", this.password);
        return params.toString();
    }
    public String getToken(String webserverIp, String webserverPort) throws Exception{
        String token = "";
        String apiUrl = ApiInfo.getLoginUrl(webserverIp, webserverPort);
        BufferedReader in = null;
        HttpURLConnection connection = APIClient.httpResponse(apiUrl, "", this.getData(), "POST");
        //获取响应结果
        int responseCode = connection.getResponseCode();
        System.out.println("getToken Response Code : " + responseCode);
        try {
            in = new BufferedReader(new InputStreamReader(connection.getInputStream(), StandardCharsets.UTF_8));
            String inputLine;
            StringBuilder response = new StringBuilder();
            while ((inputLine = in.readLine()) != null) {
                response.append(inputLine);
            }
            token = connection.getHeaderField("token");
            if (responseCode == 400) {
                System.out.println("HTTP_400_BAD_REQUEST");
            }
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            try {
                if (in != null) {
                    in.close();
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        return token;
    }

}
