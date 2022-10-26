import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;

public class TestMemoryLeakKo {
    public static HashSet<List<Person>> hashSetPerson = new HashSet<>();

    public static void main(String[] args) {
        boolean flag = true;
        Thread thread = new Thread(() -> {
            while (flag) {
                List<Person> list = new ArrayList<>(1000);
                for (int i = 0; i < 1000; i++) {
                    list.add(new Person());
                }
                hashSetPerson.add(list);
                try {
                    Thread.sleep(2000L);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        });
        thread.start();
    }

    public static class Person {
        String name = "abc";
        int age = 11;
        byte[] b = new byte[1024 * 5];
    }
}