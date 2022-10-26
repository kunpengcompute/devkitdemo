public class Lock {
    private static Lock lock = new Lock();

    public static void main(String[] args) throws InterruptedException {
        while (true) {
            for (int i = 0; i < 10; i++) {
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        synchronized (lock) {
                            for (int j = 0; j < 1000000; j++) {

                            }
                        }
                    }
                }).start();
            }
            Thread.sleep(2000L);
        }
    }
}