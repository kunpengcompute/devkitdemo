import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.atomic.AtomicBoolean;

public class TraceDemo {
    private static final int DATA_SIZE = 1024;
    private static final int JAVA_COMPUTE_ROUNDS = 120_000;
    private static final int NATIVE_COMPUTE_ROUNDS = 200_000;
    private static final int WRITE_COUNT = 16;

    public static void main(String[] args) throws Exception {
        int durationSeconds = args.length > 0 ? Integer.parseInt(args[0]) : 60;
        int workers = args.length > 1 ? Integer.parseInt(args[1]) : 4;

        System.out.println("========================================");
        System.out.println("Java + Native Pattern Demo");
        System.out.println("duration : " + durationSeconds + " s");
        System.out.println("workers  : " + workers);
        System.out.println("pid      : " + ProcessHandle.current().pid());
        System.out.println("========================================");

        AtomicBoolean running = new AtomicBoolean(true);
        CountDownLatch ready = new CountDownLatch(workers);
        CountDownLatch start = new CountDownLatch(1);
        List<Thread> threads = new ArrayList<>();

        for (int i = 0; i < workers; ++i) {
            final int workerId = i;

            Thread thread = new Thread(
                    () -> runWorker(workerId, running, ready, start),
                    "trace-worker-" + i);

            threads.add(thread);
            thread.start();
        }

        ready.await();
        System.out.println("All workers ready.");
        start.countDown();

        Thread.sleep(durationSeconds * 1000L);
        running.set(false);

        for (Thread thread : threads) {
            thread.join();
        }

        System.out.println("Workload finished.");
    }

    private static void runWorker(
            int workerId,
            AtomicBoolean running,
            CountDownLatch ready,
            CountDownLatch start) {

        byte[] data = createData(workerId);
        ready.countDown();

        try {
            start.await();
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            return;
        }

        long count = 0;
        long result = 0;

        while (running.get()) {
            result ^= javaHotProcess(data, count);
            ++count;
        }
        System.out.printf("[%s] calls=%d result=%d%n", Thread.currentThread().getName(), count, result);
    }

    private static long javaHotProcess(byte[] data, long requestId) {
        long value = requestId ^ data[0] ^ data[data.length - 1];

        for (int i = 0; i < JAVA_COMPUTE_ROUNDS; ++i) {
            value ^= value << 13;
            value ^= value >>> 7;
            value ^= value << 17;
            value += (long) i * 17L;
            value ^= data[i & (DATA_SIZE - 1)];
        }

        long nativeResult = NativeBridge.process(data, NATIVE_COMPUTE_ROUNDS, WRITE_COUNT);

        return value ^ nativeResult;
    }

    private static byte[] createData(int workerId) {
        byte[] data = new byte[DATA_SIZE];

        for (int i = 0; i < data.length; ++i) {
            data[i] = (byte) ((i * 31 + workerId) & 0xff);
        }

        return data;
    }
}