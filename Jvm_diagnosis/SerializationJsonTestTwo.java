import org.json.JSONArray;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.List;

/**
 * Parsing the JSON File Format
 */
public class SerializationJsonTestTwo {

    public static List<TestInfo> deserializeJson(JSONArray jsonArray) {
        List<TestInfo> testInfos = new ArrayList<>();
        for (int i = 0; i < jsonArray.length(); i++) {
            JSONObject jsonObject = jsonArray.getJSONObject(i);
            TestInfo testInfo = new TestInfo();
            testInfo.setName(jsonObject.getString("name"));
            testInfo.setLanguage(jsonObject.getString("language"));
            testInfo.setBio(jsonObject.getString("bio"));
            testInfo.setId(jsonObject.getString("id"));
            testInfo.setVersion(jsonObject.getDouble("version"));
            testInfos.add(testInfo);
        }
        return testInfos;
    }

    public static String serializeJson(List<TestInfo> testInfos) {
        JSONArray jsonArray = new JSONArray();
        for (TestInfo testInfo : testInfos) {
            JSONObject jsonObject = new JSONObject();
            jsonObject.put("name", testInfo.getName());
            jsonObject.put("language", testInfo.getLanguage());
            jsonObject.put("id", testInfo.getId());
            jsonObject.put("version", testInfo.getVersion());
            jsonArray.put(jsonObject);
        }
        return jsonArray.toString();
    }

    private static String getCurrentTime() {
        LocalDateTime now = LocalDateTime.now();
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("HH:mm:ss.SSS");
        return now.format(formatter);
    }

    private static String readFile(String filePath) {
        StringBuilder contentBuilder = new StringBuilder();
        try (BufferedReader br = new BufferedReader(new FileReader(filePath))) {
            String currentLine;
            while ((currentLine = br.readLine()) != null) {
                contentBuilder.append(currentLine).append(" ");
            }
        } catch (IOException e) {
            e.printStackTrace();
            return null;
        }
        return contentBuilder.toString();
    }
}

class TestInfo {
    private String name;
    private String language;
    private String bio;
    private String id;
    private Double version;

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public Double getVersion() {
        return version;
    }

    public void setVersion(Double version) {
        this.version = version;
    }

    public String getLanguage() {
        return language;
    }

    public void setLanguage(String language) {
        this.language = language;
    }

    public String getBio() {
        return bio;
    }

    public void setBio(String bio) {
        this.bio = bio;
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    @Override
    public String toString() {
        return "TestInfo{" +
                "name='" + name + '\'' +
                ", language='" + language + '\'' +
                ", bio='" + bio + '\'' +
                ", id='" + id + '\'' +
                ", version=" + version +
                '}';
    }
}
