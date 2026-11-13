import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;
import java.util.concurrent.atomic.AtomicBoolean;

public class OrderPipelineDemo extends NativeBridge {

    private static final int DATA_SIZE = 1024;

    /** JVM PID（兼容 Java 9+ 和 Java 8） */
    private static final long PID = getPid();

    /** 共享停止标志 */
    private static final AtomicBoolean STOP = new AtomicBoolean(false);

    public static void main(String[] args) throws Exception {
        int durationSeconds = args.length > 0 ? Integer.parseInt(args[0]) : 60;
        int workerCount = args.length > 1 ? Integer.parseInt(args[1]) : 4;

        System.out.println("=== OrderPipelineDemo ===");
        System.out.println("duration  : " + durationSeconds);
        System.out.println("workers   : " + workerCount);
        System.out.println("pid       : " + PID);

        CyclicBarrier barrier = new CyclicBarrier(workerCount + 1);
        Thread[] workers = new Thread[workerCount];
        WorkerResult[] results = new WorkerResult[workerCount];

        // 启动 worker 线程
        for (int i = 0; i < workerCount; i++) {
            final int idx = i;
            results[i] = new WorkerResult();
            workers[i] = new Thread(() -> runWorker(idx, barrier, results[idx]),
                    "order-worker-" + i);
            workers[i].start();
        }

        // 等所有 worker 就绪
        barrier.await();
        System.out.println("All workers ready, start pipeline...");

        // 主线程计时
        Thread.sleep(durationSeconds * 1000L);
        STOP.set(true);
        System.out.println("Stop signal sent, joining workers...");

        for (Thread w : workers) {
            w.join();
        }

        // 汇总打印
        long totalOrders = 0;
        long totalAccum = 0;
        for (int i = 0; i < workerCount; i++) {
            WorkerResult r = results[i];
            System.out.printf("[%s] orders=%d  accumulated=%d%n",
                    workers[i].getName(), r.orderCount, r.accumulatedValue);
            totalOrders += r.orderCount;
            totalAccum += r.accumulatedValue;
        }
        System.out.println("=== Done ===");
        System.out.println("Total orders      : " + totalOrders);
        System.out.println("Total accumulated : " + totalAccum);
        System.out.println("Run finished.");
    }

    private static void runWorker(int idx, CyclicBarrier barrier, WorkerResult result) {
        try {
            barrier.await(); // 向主线程报到并等待开始信号
        } catch (InterruptedException | BrokenBarrierException e) {
            return;
        }

        long localOrderId = (long) idx << 40; // 每个 worker 独立 ID 空间，防冲突
        byte[] buf = new byte[DATA_SIZE];
        // 把 buffer 填点非零值，避免 JIT 完全消除计算
        for (int i = 0; i < DATA_SIZE; i++) buf[i] = (byte) (i & 0xFF);

        long orderCount = 0;
        long accumulated = 0;

        while (!STOP.get()) {
            long orderId = ++localOrderId;
            long ret = processOrder(buf, DATA_SIZE, orderId);
            accumulated += ret;
            orderCount++;
        }

        result.orderCount = orderCount;
        result.accumulatedValue = accumulated;
    }

    /**
     * 单订单处理流程：校验（Java 侧） → 定价 → 扣库存 → 持久化（后三步 JNI native）。
     */
    private static long processOrder(byte[] data, int size, long orderId) {
        // 1) 校验（Java 侧，非 JNI）
        if (data == null || size <= 0 || size > data.length) {
            return -1;
        }
        if (orderId <= 0) {
            return -1;
        }
        long checksum = (orderId ^ (orderId >>> 32)) & 0xFFFFL;

        // 2) 定价（JNI） —— calcPrice，CPU 热点
        long price = calcPrice(data, size, orderId);

        // 3) 扣库存（JNI） —— checkStock，锁竞争
        long remaining = checkStock(data, size, orderId);
        if (remaining < 0) {
            return -2; // 库存不足
        }

        // 4) 持久化（JNI） —— asyncPersist，p99 长尾
        long written = asyncPersist(data, size, orderId, price);

        return price + written + checksum;
    }

    private static long getPid() {
        try {
            // Java 9+
            return ProcessHandle.current().pid();
        } catch (Throwable t) {
            try {
                // Java 8 回退
                Class<?> c = Class.forName("java.lang.management.ManagementFactory");
                Object runtime = c.getMethod("getRuntimeMXBean").invoke(null);
                String name = (String) runtime.getClass().getMethod("getName").invoke(runtime);
                return Long.parseLong(name.split("@")[0]);
            } catch (Throwable t2) {
                return -1;
            }
        }
    }

    /** 每个 worker 的退出统计结果 */
    static class WorkerResult {
        long orderCount;
        long accumulatedValue;
    }
}
