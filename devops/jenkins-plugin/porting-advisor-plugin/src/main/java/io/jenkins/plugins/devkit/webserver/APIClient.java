package io.jenkins.plugins.devkit.webserver;


import jenkins.security.CustomClassFilter;

import javax.net.ssl.SSLContext;
import java.io.DataOutputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import javax.net.ssl.HttpsURLConnection;
import javax.net.ssl.TrustManager;
import javax.net.ssl.X509TrustManager;
import java.security.cert.X509Certificate;
import java.util.Arrays;


public class APIClient {
    public static HttpURLConnection httpResponse(String apiUrl, String apiToken, String apiData, String requestMethod) throws Exception {
        // Create a trust manager that does not validate certificate chains
        TrustManager[] trustAllCerts = new TrustManager[]{new X509TrustManager() {
            public X509Certificate[] getAcceptedIssuers() {
                return null;
            }
            public void checkClientTrusted(X509Certificate[] certs, String authType) {
            }

            public void checkServerTrusted(X509Certificate[] certs, String authType) {
            }
        }};

        // Install the all-trusting trust manager
        SSLContext sslContext = SSLContext.getInstance("SSL");
        sslContext.init(null, trustAllCerts, new java.security.SecureRandom());
        HttpsURLConnection.setDefaultSSLSocketFactory(sslContext.getSocketFactory());

        // Create connection
        URL url = new URL(apiUrl);
        HttpURLConnection connection = (HttpURLConnection) url.openConnection();

        connection.setRequestMethod(requestMethod);
        connection.setRequestProperty("accept", "*/*");
        connection.setRequestProperty("Content-Type", "application/json");
        connection.setRequestProperty("Authorization", apiToken);
        connection.setDoOutput(true);
        connection.setDoInput(true);

        // Send request
        if (requestMethod.equals("POST")) {
            DataOutputStream wr = new DataOutputStream(connection.getOutputStream());
            wr.writeBytes(apiData);
            wr.flush();
            wr.close();
        }

        return connection;
    }


    public static Boolean ApiErrCode(String code) {
        String[] errorCodes = {
                "0x01011e", "0x010611",
                "0x010599", "0x0c0310",
                "0x0c0311", "0x050512",
                "0x010416", "0x0c0312",
                "0x050314", "0x010811",
                "0x050519", "0x0c0314",
                "0x05051a", "0x0c0315",
                "0x0c0210", "0x0c0316",
                "0x05051b", "0x0c0317",
                "0x0d0110", "0x0c0318",
                "0x0d0203", "0x0c0211",
                "0x0d0204", "0x0c0110",
                "0x0d0205", "0x0d0206",
                "0x0d0209", "0x0d020b",
                "0x0d020c", "0x0d020e",
                "0x0d020f", "0x0d020g",
                "0x0d0210", "0x0d0211",
                "0x0d0213", "0x0d0214",
                "0x0d0215", "0x0d0216",
                "0x0d0219", "0x0d021a",
                "0x0d021b", "0x0d021c",
                "0x0d021e", "0x0d021f",
                "0x0d0221", "0x0d0223",
                "0x0d0208", "0x0d0218"

        };
        for (String codeNum: errorCodes) {
            if (codeNum.equals(code)) {
                return false;
            }
        }
        return true;
    }
}
