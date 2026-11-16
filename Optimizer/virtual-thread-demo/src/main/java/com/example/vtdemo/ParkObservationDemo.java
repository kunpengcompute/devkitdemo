package com.example.vtdemo;

import java.lang.management.ManagementFactory;
import java.lang.management.ThreadInfo;
import java.lang.management.ThreadMXBean;
import java.time.Duration;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Map;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.locks.LockSupport;
import java.util.concurrent.locks.ReentrantLock;

/**
 * 观测虚拟线程是否真的 park(unmount)了。三种方法:
 *
 * 方法1: 在阻塞期间从外部采样 Thread.getState() -> WAITING / TIMED_WAITING
 *        !! 注意: 只能证明"线程被阻塞", 不能证明"虚拟线程已卸载" !!
 *        park 和 pin 两种场景下 Thread.getState() 都返回 WAITING, 无法区分。
 *
 * 方法2: (决定性证据) 在阻塞期间采样载体线程(ForkJoinPool worker)的栈
 *        - 载体栈出现 ForkJoinPool.awaitWork -> 载体空闲等任务 -> VT 已卸载 -> park 生效
 *        - 载体栈出现 Continuation.run -> 载体卡在 VT 续体里 -> VT 没卸载 -> pin 发生
 *        这是唯一能直接证明 park/unmount 的手段。
 *
 * 方法3: 用 N 个虚拟线程同时阻塞,看 wall time (宏观间接证据)
 *        - park: wall ≈ 单个阻塞时间(并发完成, 载体线程被复用)
 *        - pin : wall ≈ N * 单个时间 / 载体数(串行化, 载体被钉死)
 *
 * 运行:
 *   java -Djdk.tracePinnedThreads=short -cp target/classes com.example.vtdemo.ParkObservationDemo
 */
public final class ParkObservationDemo {

    private static final ThreadMXBean TMX = ManagementFactory.getThreadMXBean();

    public static void main(String[] args) throws Exception {
        System.out.printf("java: %s  cpuCores=%d  carrierParallelism=%d%n%n",
                System.getProperty("java.version"),
                Runtime.getRuntime().availableProcessors(),
                parallelism());

        method1_sampleState("park 场景: LockSupport.park", () -> {
            Thread me = Thread.currentThread();
            Thread.ofPlatform().start(() -> {
                LockSupport.parkNanos(300_000_000L);
                LockSupport.unpark(me);
            });
            LockSupport.park();
        });

        method1_sampleState("park 场景: ReentrantLock.lock", () -> {
            ReentrantLock lock = new ReentrantLock();
            CountDownLatch holderReady = new CountDownLatch(1);
            Thread h = Thread.ofPlatform().start(() -> {
                lock.lock();
                holderReady.countDown();
                LockSupport.parkNanos(300_000_000L);
                lock.unlock();
            });
            try { holderReady.await(); } catch (InterruptedException ignored) {}
            lock.lock();
            lock.unlock();
            h.join();
        });

        method1_sampleState("park 场景: Thread.sleep", () -> Thread.sleep(300));

        method1_sampleState("pin  场景: synchronized + Thread.sleep", () -> {
            synchronized (new Object()) {
                Thread.sleep(300);
            }
        });

        method1_sampleState("pin  场景: synchronized + LockSupport.park", () -> {
            Thread me = Thread.currentThread();
            Thread.ofPlatform().start(() -> {
                LockSupport.parkNanos(300_000_000L);
                LockSupport.unpark(me);
            });
            synchronized (new Object()) {
                LockSupport.park();
            }
        });

        System.out.println(">>> 方法1 结论: park 和 pin 场景下 Thread.getState() 都是 WAITING/TIMED_WAITING,");
        System.out.println(">>> 无法区分。要证明虚拟线程真的卸载(unmount)了, 必须看方法2的载体线程栈。");

        System.out.println();
        method4_provePark("park 场景: Thread.sleep (证明 VT 已卸载)");
        method4_provePark("pin  场景: synchronized + Thread.sleep (证明 VT 未卸载)");

        System.out.println();
        // park 用 200 个 VT: 即使 VT 多,卸载后也只需少量载体
        method2_carrierSnapshot("park: 200 个 VT 同时 sleep 300ms", 200, false);

        // pin 用 parallelism 个 VT: 保证全部同批并发,快照时能同时观察到全部被钉死
        int pinN = Math.max(8, parallelism());
        method2_carrierSnapshot("pin : " + pinN + " 个 VT synchronized+sleep 300ms", pinN, true);

        System.out.println();
        method3_wallTime("park: 500 个 VT sleep 300ms", 500, false);
        method3_wallTime("pin : 500 个 VT synchronized+sleep 300ms", 500, true);
    }

    // ==================== 方法1: 采样 Thread.State ====================
    // !! 注意: 只能证明"被阻塞", 不能证明"已卸载(park)" !!
    // park 和 pin 都会显示 WAITING/TIMED_WAITING, 无法区分。

    @FunctionalInterface
    private interface Blocker {
        void block() throws Exception;
    }

    private static void method1_sampleState(String title, Blocker blocker) throws Exception {
        System.out.println("=== 方法1: 采样线程状态 (仅能证明阻塞, 不能证明卸载) === " + title);
        try (ExecutorService exec = Executors.newVirtualThreadPerTaskExecutor()) {
            List<String> events = java.util.Collections.synchronizedList(new ArrayList<>());
            Thread vt = Thread.ofVirtual().unstarted(() -> {
                try {
                    events.add(formatEvent("during-before: " + stateStr(Thread.currentThread())));
                    blocker.block();
                    events.add(formatEvent("during-after:  " + stateStr(Thread.currentThread())));
                } catch (Exception e) {
                    throw new RuntimeException(e);
                }
            });
            vt.start();
            for (int i = 0; i < 3; i++) {
                LockSupport.parkNanos(80_000_000L);
                events.add(formatEvent("sample-" + i + ":      state=" + vt.getState()
                        + " alive=" + vt.isAlive()
                        + " virtual=" + vt.isVirtual()));
            }
            vt.join();
            events.forEach(e -> System.out.println("  " + e));
        }
        System.out.println();
    }

    private static String formatEvent(String msg) {
        SimpleDateFormat sdf = new SimpleDateFormat("HH:mm:ss.SSS");
        return String.format("[%s] %s", sdf.format(new Date()), msg);
    }

    // ==================== 方法4: 证明 park 是否发生 (VT + 载体配对采样) ====================

    private static void method4_provePark(String title) throws Exception {
        boolean isPin = title.contains("pin");
        System.out.println("=== 方法4: 证明 park 是否发生 === " + title);
        CountDownLatch vtBlocked = new CountDownLatch(1);
        Thread vt = Thread.ofVirtual().unstarted(() -> {
            try {
                vtBlocked.countDown();
                if (isPin) {
                    synchronized (new Object()) {
                        Thread.sleep(300);
                    }
                } else {
                    Thread.sleep(300);
                }
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        });
        vt.start();
        vtBlocked.await();
        LockSupport.parkNanos(50_000_000L);
        for (int i = 0; i < 3; i++) {
            LockSupport.parkNanos(80_000_000L);
            String vtState = vt.getState().toString();
            String carrierInfo = findCarrierForVT();
            System.out.printf("  [%s] VT: state=%-15s | 载体: %s%n",
                    new SimpleDateFormat("HH:mm:ss.SSS").format(new Date()),
                    vtState, carrierInfo);
        }
        vt.join();
        System.out.println();
    }

    private static String findCarrierForVT() {
        // 检查是否有载体在执行 VT 的续体 (Continuation.run)
        Map<Thread, StackTraceElement[]> all = Thread.getAllStackTraces();
        int pinnedCarriers = 0;
        for (Map.Entry<Thread, StackTraceElement[]> e : all.entrySet()) {
            Thread t = e.getKey();
            String name = t.getName();
            if (!name.startsWith("ForkJoinPool-1-worker")) continue;
            StackTraceElement[] st = e.getValue();
            // 检查这个载体是否在执行 VT 的续体
            for (StackTraceElement f : st) {
                if (f.getClassName().equals("jdk.internal.vm.Continuation")) {
                    pinnedCarriers++;
                    break;
                }
            }
        }
        if (pinnedCarriers > 0) {
            return "被钉死 (" + pinnedCarriers + " 个载体在 Continuation.run)";
        } else {
            return "已卸载 (无载体在执行 VT 续体) → park 生效";
        }
    }

    // ==================== 方法2: 载体线程快照 (决定性证据) ====================

    private static void method2_carrierSnapshot(String title, int n, boolean pin) throws Exception {
        System.out.println("=== 方法2: 载体线程快照 (决定性证据: 看载体栈才能证明 VT 卸载) === " + title);
        CountDownLatch ready = new CountDownLatch(n);
        AtomicInteger carrierSeen = new AtomicInteger();
        try (ExecutorService exec = Executors.newVirtualThreadPerTaskExecutor()) {
            List<Future<?>> futures = new ArrayList<>();
            for (int i = 0; i < n; i++) {
                futures.add(exec.submit(() -> {
                    ready.countDown();
                    try {
                        if (pin) {
                            // 每个 VT 用自己的 monitor,这样每个 VT 独立 pin 自己的载体
                            // (避免共享 monitor 导致全部串行化)
                            Object m = new Object();
                            synchronized (m) {
                                Thread.sleep(300);
                            }
                        } else {
                            Thread.sleep(300);
                        }
                    } catch (InterruptedException e) {
                        Thread.currentThread().interrupt();
                    }
                }));
            }
            ready.await();
            LockSupport.parkNanos(50_000_000L);
            snapshotAllThreads(carrierSeen);
            for (Future<?> f : futures) f.get();
        }
        System.out.println();
    }

    private static void snapshotAllThreads(AtomicInteger carrierSeen) {
        // ForkJoinPool-1-worker-* 是虚拟线程的载体
        //
        // 最可靠的判别信号: 载体线程的栈顶帧
        //   - park 生效: 载体栈出现 ForkJoinPool.awaitWork -> 载体空闲等任务,VT 已卸载
        //   - pin  发生: 载体栈出现 jdk.internal.vm.Continuation.run -> 载体被 VT 的续体卡住无法卸载
        //
        // 为什么不看 "用户代码帧": pinned VT 的 Java 栈属于续体(挂载在 VirtualThread 对象下),
        // 载体栈只暴露 Continuation.run 这一层,看不到 synchronized 块内的用户帧。
        Map<Thread, StackTraceElement[]> all = Thread.getAllStackTraces();
        int carriers = 0, carrierIdle = 0, carrierPinned = 0, carrierOther = 0;
        int virtuals = 0, virtualsWaiting = 0;
        StackTraceElement[] sampleIdleStack = null;
        StackTraceElement[] samplePinnedStack = null;
        for (Map.Entry<Thread, StackTraceElement[]> e : all.entrySet()) {
            Thread t = e.getKey();
            String name = t.getName();
            StackTraceElement[] st = e.getValue();
            if (!name.startsWith("ForkJoinPool-1-worker")) {
                if (t.isVirtual()) {
                    virtuals++;
                    if (t.getState() == Thread.State.WAITING || t.getState() == Thread.State.TIMED_WAITING) {
                        virtualsWaiting++;
                    }
                }
                continue;
            }
            carriers++;
            boolean inContinuation = false;
            boolean inAwaitWork = false;
            for (StackTraceElement f : st) {
                String cn = f.getClassName();
                if (cn.equals("jdk.internal.vm.Continuation")) inContinuation = true;
                if (cn.startsWith("java.util.concurrent.ForkJoinPool")
                        && f.getMethodName().contains("awaitWork")) inAwaitWork = true;
            }
            if (inContinuation) {
                carrierPinned++;
                if (samplePinnedStack == null) samplePinnedStack = st;
            } else if (inAwaitWork || st.length == 0) {
                carrierIdle++;
                if (sampleIdleStack == null) sampleIdleStack = st;
            } else {
                carrierOther++;
            }
        }
        int parallelism = parallelism();
        System.out.printf("  载体线程(ForkJoinPool-1-worker): total=%d / parallelism=%d%n",
                carriers, parallelism);
        System.out.printf("    栈分析: 空闲(awaitWork)=%d  被续体卡住(Continuation.run)=%d  其它=%d%n",
                carrierIdle, carrierPinned, carrierOther);
        System.out.printf("  虚拟线程(仍挂载在载体上): total=%d  WAITING/TIMED_WAITING=%d%n",
                virtuals, virtualsWaiting);
        if (sampleIdleStack != null) {
            System.out.println("  样例[空闲]载体栈:");
            for (int i = 0; i < Math.min(5, sampleIdleStack.length); i++)
                System.out.printf("      at %s%n", sampleIdleStack[i]);
        }
        if (samplePinnedStack != null) {
            System.out.println("  样例[被钉死]载体栈:");
            for (int i = 0; i < Math.min(5, samplePinnedStack.length); i++)
                System.out.printf("      at %s%n", samplePinnedStack[i]);
        }
        String verdict;
        if (carrierPinned > 0) {
            verdict = String.format("发生 pin: %d 个载体卡在 Continuation.run (VT 没能卸载)", carrierPinned);
        } else if (carriers >= parallelism - 1) {
            verdict = "载体数达 parallelism 上限但无 Continuation.run (可能快照时机偏早)";
        } else {
            verdict = String.format("park 生效: 载体空闲等任务 (awaitWork),VT 已卸载; 仅需 %d 个载体", carriers);
        }
        System.out.printf("  关键解读: %s%n", verdict);
    }

    // ==================== 方法3: wall time 对比 ====================

    private static void method3_wallTime(String title, int n, boolean pin) throws Exception {
        System.out.println("=== 方法3: wall time (宏观间接证据) === " + title);
        long t0 = System.nanoTime();
        try (ExecutorService exec = Executors.newVirtualThreadPerTaskExecutor()) {
            List<Future<?>> futures = new ArrayList<>();
            for (int i = 0; i < n; i++) {
                futures.add(exec.submit(() -> {
                    try {
                        if (pin) {
                            Object m = new Object();
                            synchronized (m) {
                                Thread.sleep(300);
                            }
                        } else {
                            Thread.sleep(300);
                        }
                    } catch (InterruptedException e) {
                        Thread.currentThread().interrupt();
                    }
                }));
            }
            for (Future<?> f : futures) f.get();
        }
        long wallMs = (System.nanoTime() - t0) / 1_000_000;
        String verdict = wallMs < 600 ? "并发完成(park 生效,载体线程复用)"
                : wallMs < 2000 ? "部分串行"
                : "严重串行(pin 导致载体被钉死)";
        System.out.printf("  wall=%dms  (单任务 300ms × %d 个)  => %s%n", wallMs, n, verdict);
    }

    // ==================== 工具 ====================

    private static String stateStr(Thread t) {
        return "state=" + t.getState() + " virtual=" + t.isVirtual();
    }

    private static int parallelism() {
        try {
            return ((java.util.concurrent.ForkJoinPool) java.util.concurrent.ForkJoinPool.commonPool()).getParallelism();
        } catch (Throwable t) {
            return -1;
        }
    }
}
