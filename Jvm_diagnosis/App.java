import org.json.JSONArray;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

/**
 * Serialization and deserialization files
 */
public class App {
    public static void main(String[] args) {
        Random random = new Random();
        String[] fileNames = {"5MB.json", "users_1k.json", "users_10k.json", "users_100k.json"};

        System.out.println("begin json serialization, start time: " + getCurrentTime());
        String filePath = "./jsonData/";
        String jsonString = null;
        long start = System.currentTimeMillis();

        try {
            Thread.sleep(15000);
            int iterations = fileNames.length;

            for (int i = 0; i < iterations; i++) {
                System.out.println("number " + (i + 1) + " deserializeJson:");
                jsonString = readFile(filePath + fileNames[i]);

                if (fileNames[i].equals("5MB.json")) {
                    List<TestInfo> testInfo = SerializationJsonTestTwo.deserializeJson(new JSONArray(jsonString));
                    String serializeJson = SerializationJsonTestTwo.serializeJson(testInfo);
                } else {
                    List<Person> people = deserializeJson(jsonString);
                    String serializeJson = serializeJson(people);
                }

                System.out.println("end json serializeJson, end time " + getCurrentTime());
            }

            long end = System.currentTimeMillis();
            long timespend = end - start;
            System.out.println("timespend " + timespend);

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private static String getCurrentTime() {
        LocalDateTime now = LocalDateTime.now();
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("HH:mm:ss.SSS");
        return now.format(formatter);
    }

    private static List<Person> deserializeJson(String jsonString) {
        List<Person> people = new ArrayList<>();
        JSONArray jsonArray = new JSONArray(jsonString);

        for (int i = 0; i < jsonArray.length(); i++) {
            JSONObject jsonObject = jsonArray.getJSONObject(i);
            Person person = new Person();
            person.setId(jsonObject.getInt("id"));
            person.setName(jsonObject.getString("name"));
            person.setCity(jsonObject.getString("city"));
            person.setAge(jsonObject.getInt("age"));

            JSONArray friends = jsonObject.getJSONArray("friends");
            for (int j = 0; j < friends.length(); j++) {
                JSONObject friendObj = friends.getJSONObject(j);
                Friend friend = new Friend();
                friend.setName(friendObj.getString("name"));

                JSONArray hobbies = friendObj.getJSONArray("hobbies");
                for (int k = 0; k < hobbies.length(); k++) {
                    friend.getHobbies().add(hobbies.getString(k));
                }
                person.getFriends().add(friend);
            }
            people.add(person);
        }
        return people;
    }

    private static String serializeJson(List<Person> people) {
        JSONArray jsonArray = new JSONArray();
        for (Person person : people) {
            JSONObject jsonObject = new JSONObject();
            jsonObject.put("id", person.getId());
            jsonObject.put("name", person.getName());
            jsonObject.put("age", person.getAge());
            jsonObject.put("city", person.getCity());

            JSONArray friendArray = new JSONArray();
            for (Friend friend : person.getFriends()) {
                JSONObject friendObj = new JSONObject();
                friendObj.put("name", friend.getName());

                JSONArray hobbiesArray = new JSONArray();
                for (String hobby : friend.getHobbies()) {
                    hobbiesArray.put(hobby);
                }
                friendObj.put("hobbies", hobbiesArray);
                friendArray.put(friendObj);
            }
            jsonObject.put("friends", friendArray);
            jsonArray.put(jsonObject);
        }
        return jsonArray.toString();
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

class Person {
    private int id;
    private String name;
    private String city;
    private int age;
    private List<Friend> friends;

    public Person() {
        friends = new ArrayList<>();
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getCity() {
        return city;
    }

    public void setCity(String city) {
        this.city = city;
    }

    public int getAge() {
        return age;
    }

    public void setAge(int age) {
        this.age = age;
    }

    public List<Friend> getFriends() {
        return friends;
    }

    public void setFriends(List<Friend> friends) {
        this.friends = friends;
    }

    @Override
    public String toString() {
        return "Person{" +
                "id=" + id +
                ", name='" + name + '\'' +
                ", city='" + city + '\'' +
                ", age=" + age +
                ", friends=" + friends +
                '}';
    }
}

class Friend {
    private String name;
    private List<String> hobbies;

    public Friend() {
        hobbies = new ArrayList<>();
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public List<String> getHobbies() {
        return hobbies;
    }
}