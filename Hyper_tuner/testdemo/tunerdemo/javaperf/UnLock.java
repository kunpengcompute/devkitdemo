import java.util.concurrent.locks.ReentrantLock;

public class UnLock {
    private static final ReentrantLock lock = new ReentrantLock();
    public static void main(String[] args) throws Exception {
        Thread t = new Thread(() -> {
            lock.lock();
            try {
                Thread.sleep(10000000L);
            } catch (Exception e) {
                e.printStackTrace();
            } finally {
                lock.unlock();
            }
        });
        t.start();
        for (int i = 0; i < 50; i++) {
            new Thread(() -> {
                if (lock.tryLock()) {
                    try {
                        System.out.println("等");
                    } finally {
                        lock.unlock();
                    }
                } 
            }).start();
        }
    }
}
