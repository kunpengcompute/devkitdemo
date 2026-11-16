package com.example.vtdemo;

import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.charset.StandardCharsets;
import java.time.Duration;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.CyclicBarrier;
import java.util.concurrent.Exchanger;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.Phaser;
import java.util.concurrent.Semaphore;
import java.util.concurrent.SynchronousQueue;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicReference;
import java.util.concurrent.locks.LockSupport;
import java.util.concurrent.locks.ReentrantLock;
import java.util.concurrent.locks.ReentrantReadWriteLock;
import java.util.concurrent.locks.StampedLock;

/**
 * 验证虚拟线程中各类阻塞操作的 park / pin 行为。
 *
 * 运行：
 *   java -Djdk.tracePinnedThreads=short -jar target/vtdemo.jar park
 *   mvn exec:java -Dexec.mainClass=com.example.vtdemo.ParkScenariosDemo -Dexec.args="park"
 *
 * 关键观察：
 *   - park 场景：Thread.State = WAITING / TIMED_WAITING，载体线程仍可调度其他虚拟线程
 *   - pin  场景：synchronized 块内阻塞会打印 "Thread ... is pinned" 警告
 */
public final class ParkScenariosDemo {

    private static volatile boolean stop = false;

    public static void main(String[] args) throws Exception {
        System.out.printf("java: %s thread=%s%n", System.getProperty("java.version"), threadName());
        System.out.println("tracePinnedThreads=" + System.getProperty("jdk.tracePinnedThreads"));
        System.out.println("=".repeat(80));

        runScenario("1. LockSupport.park", ParkScenariosDemo::lockSupportPark);
        runScenario("2. ReentrantLock.lock", ParkScenariosDemo::reentrantLock);
        runScenario("3. ReentrantReadWriteLock", ParkScenariosDemo::readWriteLock);
        runScenario("4. StampedLock", ParkScenariosDemo::stampedLock);
        runScenario("5. Condition.await", ParkScenariosDemo::conditionAwait);
        runScenario("6. Semaphore.acquire", ParkScenariosDemo::semaphoreAcquire);
        runScenario("7. CountDownLatch.await", ParkScenariosDemo::countDownLatch);
        runScenario("8. CyclicBarrier.await", ParkScenariosDemo::cyclicBarrier);
        runScenario("9. Phaser.arriveAndAwaitAdvance", ParkScenariosDemo::phaser);
        runScenario("10. Exchanger.exchange", ParkScenariosDemo::exchanger);
        runScenario("11. SynchronousQueue (put/take)", ParkScenariosDemo::synchronousQueue);
        runScenario("12. LinkedBlockingQueue (take/put)", ParkScenariosDemo::linkedBlockingQueue);
        runScenario("13. Thread.sleep", ParkScenariosDemo::threadSleep);
        runScenario("14. Object.wait (synchronized pin)", ParkScenariosDemo::objectWait);
        runScenario("15. synchronized + IO (pin)", ParkScenariosDemo::synchronizedIoPin);
        runScenario("16. Future.get", ParkScenariosDemo::futureGet);
        runScenario("17. CompletableFuture.join", ParkScenariosDemo::completableFutureJoin);
        runScenario("18. Thread.join", ParkScenariosDemo::threadJoin);
        runScenario("19. Blocking Socket IO", ParkScenariosDemo::blockingSocketIo);

        System.out.println("=".repeat(80));
        System.out.println("done");
    }

    @FunctionalInterface
    private interface Scenario {
        void run() throws Exception;
    }

    private static void runScenario(String name, Scenario scenario) {
        System.out.printf("%n--- %s ---%n", name);
        try (ExecutorService exec = Executors.newVirtualThreadPerTaskExecutor()) {
            AtomicReference<String> stateRef = new AtomicReference<>();
            Future<?> future = exec.submit(() -> {
                try {
                    stateRef.set("running on: " + threadName());
                    scenario.run();
                } catch (Exception e) {
                    stateRef.set("error: " + e);
                    throw new RuntimeException(e);
                }
            });
            // 等虚拟线程进入阻塞
            Thread.sleep(200);
            Thread[] carrier = new Thread[1];
            Thread.enumerate(carrier);
            System.out.println("  " + stateRef.get());
            System.out.println("  after block -> thread state: " + future.state()
                    + " (NOTE: Future.state() returns RUNNING/FAILED/etc, not Thread.State)");
            // 唤醒/取消
            stop = false;
            future.get(5, TimeUnit.SECONDS);
        } catch (Exception e) {
            System.out.println("  scenario failed: " + e);
        }
    }

    // ---------- 1. LockSupport.park ----------
    private static void lockSupportPark() throws Exception {
        Thread me = Thread.currentThread();
        Thread carrier = Thread.ofPlatform().start(() -> {
            try { Thread.sleep(100); } catch (InterruptedException ignored) {}
            LockSupport.unpark(me);
        });
        reportState("before park");
        LockSupport.park();
        reportState("after park");
        carrier.join();
    }

    // ---------- 2. ReentrantLock ----------
    private static void reentrantLock() throws Exception {
        ReentrantLock lock = new ReentrantLock();
        Thread holder = Thread.ofPlatform().start(() -> {
            lock.lock();
            try { Thread.sleep(500); } catch (InterruptedException ignored) {}
            finally { lock.unlock(); }
        });
        Thread.sleep(50);
        reportState("before lock");
        lock.lock();
        reportState("after lock");
        lock.unlock();
        holder.join();
    }

    // ---------- 3. ReentrantReadWriteLock ----------
    private static void readWriteLock() throws Exception {
        ReentrantReadWriteLock rw = new ReentrantReadWriteLock();
        Thread writer = Thread.ofPlatform().start(() -> {
            rw.writeLock().lock();
            try { Thread.sleep(500); } catch (InterruptedException ignored) {}
            finally { rw.writeLock().unlock(); }
        });
        Thread.sleep(50);
        reportState("before readLock");
        rw.readLock().lock();
        reportState("after readLock");
        rw.readLock().unlock();
        writer.join();
    }

    // ---------- 4. StampedLock ----------
    private static void stampedLock() throws Exception {
        StampedLock sl = new StampedLock();
        Thread writer = Thread.ofPlatform().start(() -> {
            long s = sl.writeLock();
            try { Thread.sleep(500); } catch (InterruptedException ignored) {}
            finally { sl.unlockWrite(s); }
        });
        Thread.sleep(50);
        reportState("before readLock");
        long stamp = sl.readLock();
        reportState("after readLock");
        sl.unlockRead(stamp);
        writer.join();
    }

    // ---------- 5. Condition.await ----------
    private static void conditionAwait() throws Exception {
        ReentrantLock lock = new ReentrantLock();
        var cond = lock.newCondition();
        Thread signaler = Thread.ofPlatform().start(() -> {
            try { Thread.sleep(200); } catch (InterruptedException ignored) {}
            lock.lock();
            try { cond.signal(); } finally { lock.unlock(); }
        });
        reportState("before await");
        lock.lock();
        try { cond.await(); } finally { lock.unlock(); }
        reportState("after await");
        signaler.join();
    }

    // ---------- 6. Semaphore.acquire ----------
    private static void semaphoreAcquire() throws Exception {
        Semaphore sem = new Semaphore(0);
        Thread releaser = Thread.ofPlatform().start(() -> {
            try { Thread.sleep(200); } catch (InterruptedException ignored) {}
            sem.release();
        });
        reportState("before acquire");
        sem.acquire();
        reportState("after acquire");
        releaser.join();
    }

    // ---------- 7. CountDownLatch.await ----------
    private static void countDownLatch() throws Exception {
        CountDownLatch latch = new CountDownLatch(1);
        Thread counter = Thread.ofPlatform().start(() -> {
            try { Thread.sleep(200); } catch (InterruptedException ignored) {}
            latch.countDown();
        });
        reportState("before await");
        latch.await();
        reportState("after await");
        counter.join();
    }

    // ---------- 8. CyclicBarrier.await ----------
    private static void cyclicBarrier() throws Exception {
        CyclicBarrier barrier = new CyclicBarrier(2);
        Thread party = Thread.ofPlatform().start(() -> {
            try { Thread.sleep(200); barrier.await(); } catch (Exception ignored) {}
        });
        reportState("before await");
        barrier.await();
        reportState("after await");
        party.join();
    }

    // ---------- 9. Phaser.arriveAndAwaitAdvance ----------
    private static void phaser() throws Exception {
        Phaser phaser = new Phaser(2);
        Thread party = Thread.ofPlatform().start(() -> {
            try { Thread.sleep(200); phaser.arriveAndAwaitAdvance(); } catch (Exception ignored) {}
        });
        reportState("before arrive");
        phaser.arriveAndAwaitAdvance();
        reportState("after arrive");
        party.join();
    }

    // ---------- 10. Exchanger.exchange ----------
    private static void exchanger() throws Exception {
        Exchanger<String> exchanger = new Exchanger<>();
        Thread party = Thread.ofPlatform().start(() -> {
            try { Thread.sleep(200); exchanger.exchange("B"); } catch (Exception ignored) {}
        });
        reportState("before exchange");
        String got = exchanger.exchange("A");
        reportState("after exchange: " + got);
        party.join();
    }

    // ---------- 11. SynchronousQueue ----------
    private static void synchronousQueue() throws Exception {
        SynchronousQueue<String> queue = new SynchronousQueue<>();
        Thread taker = Thread.ofPlatform().start(() -> {
            try { Thread.sleep(200); queue.take(); } catch (Exception ignored) {}
        });
        reportState("before put");
        queue.put("item");
        reportState("after put");
        taker.join();
    }

    // ---------- 12. LinkedBlockingQueue ----------
    private static void linkedBlockingQueue() throws Exception {
        LinkedBlockingQueue<String> queue = new LinkedBlockingQueue<>(1);
        queue.put("full");
        Thread putter = Thread.ofPlatform().start(() -> {
            try { Thread.sleep(200); queue.put("overflow"); } catch (Exception ignored) {}
        });
        Thread.sleep(100);
        reportState("waiting put blocked");
        queue.take();
        putter.join();
        reportState("after take");
    }

    // ---------- 13. Thread.sleep ----------
    private static void threadSleep() throws Exception {
        reportState("before sleep");
        Thread.sleep(300);
        reportState("after sleep");
    }

    // ---------- 14. Object.wait (会 pin) ----------
    private static void objectWait() throws Exception {
        Object lock = new Object();
        Thread signaler = Thread.ofPlatform().start(() -> {
            try { Thread.sleep(200); } catch (InterruptedException ignored) {}
            synchronized (lock) { lock.notify(); }
        });
        reportState("before wait");
        synchronized (lock) {
            lock.wait();
        }
        reportState("after wait");
        signaler.join();
    }

    // ---------- 15. synchronized + 阻塞 IO (会 pin) ----------
    private static void synchronizedIoPin() throws Exception {
        try (ServerSocket server = new ServerSocket()) {
            server.bind(new InetSocketAddress("127.0.0.1", 0));
            Thread serverThread = Thread.ofPlatform().start(() -> {
                try (Socket s = server.accept()) {
                    Thread.sleep(200);
                    OutputStream os = s.getOutputStream();
                    os.write("hi".getBytes(StandardCharsets.US_ASCII));
                    os.flush();
                } catch (Exception ignored) {}
            });
            Object monitor = new Object();
            reportState("before synchronized socket read");
            synchronized (monitor) {
                try (Socket s = new Socket()) {
                    s.connect(server.getLocalSocketAddress(), 5_000);
                    s.setSoTimeout(10_000);
                    InputStream in = s.getInputStream();
                    int b = in.read();  // 阻塞 IO 在 synchronized 内 -> pin
                    reportState("read byte: " + b);
                }
            }
            serverThread.join();
        }
    }

    // ---------- 16. Future.get ----------
    private static void futureGet() throws Exception {
        try (ExecutorService exec = Executors.newVirtualThreadPerTaskExecutor()) {
            Future<String> f = exec.submit(() -> {
                Thread.sleep(300);
                return "result";
            });
            reportState("before future.get");
            String r = f.get();
            reportState("after future.get: " + r);
        }
    }

    // ---------- 17. CompletableFuture.join ----------
    private static void completableFutureJoin() throws Exception {
        CompletableFuture<String> cf = new CompletableFuture<>();
        Thread completer = Thread.ofPlatform().start(() -> {
            try { Thread.sleep(300); } catch (InterruptedException ignored) {}
            cf.complete("done");
        });
        reportState("before join");
        String r = cf.join();
        reportState("after join: " + r);
        completer.join();
    }

    // ---------- 18. Thread.join ----------
    private static void threadJoin() throws Exception {
        Thread target = Thread.ofPlatform().start(() -> {
            try { Thread.sleep(300); } catch (InterruptedException ignored) {}
        });
        reportState("before join");
        target.join();
        reportState("after join");
    }

    // ---------- 19. Blocking Socket IO ----------
    private static void blockingSocketIo() throws Exception {
        try (ServerSocket server = new ServerSocket()) {
            server.bind(new InetSocketAddress("127.0.0.1", 0));
            Thread serverThread = Thread.ofPlatform().start(() -> {
                try (Socket s = server.accept()) {
                    Thread.sleep(300);
                    OutputStream os = s.getOutputStream();
                    os.write("hi".getBytes(StandardCharsets.US_ASCII));
                    os.flush();
                } catch (Exception ignored) {}
            });
            reportState("before socket read");
            try (Socket s = new Socket()) {
                s.connect(server.getLocalSocketAddress(), 5_000);
                s.setSoTimeout(10_000);
                InputStream in = s.getInputStream();
                int b = in.read();
                reportState("read byte: " + b);
            }
            serverThread.join();
        }
    }

    // ---------- 辅助方法 ----------
    private static void reportState(String tag) {
        Thread t = Thread.currentThread();
        System.out.printf("  [%s] thread=%s state=%s virtual=%b carrier=%s%n",
                tag, t.getName(), t.getState(), t.isVirtual(),
                t.isVirtual() ? "<unmountable>" : "n/a");
    }

    private static String threadName() {
        Thread t = Thread.currentThread();
        return t.getName() + " (virtual=" + t.isVirtual() + ")";
    }
}
