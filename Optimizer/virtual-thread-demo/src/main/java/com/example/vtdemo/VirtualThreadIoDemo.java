package com.example.vtdemo;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.URI;
import java.nio.file.Path;
import java.util.Arrays;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.nio.charset.StandardCharsets;
import java.time.Duration;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.ThreadLocalRandom;
import java.util.concurrent.atomic.AtomicLong;

public final class VirtualThreadIoDemo {

    public enum Mode {
        NORMAL,
        PINNED
    }

    public enum Transport {
        HTTP,
        SOCKET_READ,
        SOCKET_WRITE,
        FILE_READ,
        FILE_WRITE
    }

    public record Config(int threads, int rounds, Duration latency, Duration postSleep, Mode mode,
                         Transport transport, boolean loop) {
        public Config(int threads, int rounds, Duration latency, Duration postSleep, Mode mode,
                      Transport transport) {
            this(threads, rounds, latency, postSleep, mode, transport, false);
        }

        public static Config defaults() {
            return new Config(200, 3, Duration.ofMillis(200), Duration.ofMillis(50), Mode.NORMAL,
                    Transport.HTTP, false);
        }
    }

    @FunctionalInterface
    private interface Exchange {
        boolean exchange(int index) throws Exception;
    }

    public record Stats(long wallMillis, long tasks, long ok, long failed, long minTaskNanos, long maxTaskNanos,
                        long avgTaskNanos) {
    }

    // socket 写：交替发送随机长度 —— 奇数次 [1, 8K] 字节，偶数次 [8K, 512K] 字节（与 read 的交替机制一致）
    private static final int WRITE_SMALL_MIN = 1;
    private static final int WRITE_SMALL_MAX = 8 * 1024;
    private static final int WRITE_LARGE_MIN = 8 * 1024;
    private static final int WRITE_LARGE_MAX = 512 * 1024;

    private final AtomicLong writeCounter = new AtomicLong();

    private static final byte[] FILE_WRITE_PAYLOAD = filled(256 * 1024, (byte) 'w');

    /** 每轮之间的固定间隔。 */
    private static final Duration ROUND_INTERVAL = Duration.ofMillis(500);

    private final Config config;
    private final HttpClient client;
    private final Object[] pinMonitors;

    public VirtualThreadIoDemo(Config config) {
        this.config = config;
        this.pinMonitors = new Object[Math.max(1, config.threads())];
        for (int i = 0; i < pinMonitors.length; i++) {
            pinMonitors[i] = new Object();
        }
        this.client = HttpClient.newBuilder()
                .connectTimeout(Duration.ofSeconds(5))
                .version(HttpClient.Version.HTTP_1_1)
                .build();
    }

    public Stats run() throws IOException, ExecutionException, InterruptedException {
        return switch (config.transport()) {
            case HTTP -> {
                try (DemoHttpServer server = new DemoHttpServer(config.latency())) {
                    server.start();
                    String url = server.baseUrl() + "/io";
                    Exchange exchange = index -> httpExchange(url, index);
                    warmup(exchange);
                    yield measure(exchange);
                }
            }
            case SOCKET_READ -> {
                try (DemoSocketServer server = new DemoSocketServer(config.latency())) {
                    server.start();
                    String host = server.host();
                    int port = server.port();
                    Exchange exchange = index -> socketReadExchange(host, port, index);
                    warmup(exchange);
                    yield measure(exchange);
                }
            }
            case SOCKET_WRITE -> {
                try (DemoSocketWriteServer server = new DemoSocketWriteServer(config.latency())) {
                    server.start();
                    String host = server.host();
                    int port = server.port();
                    Exchange exchange = index -> socketWriteExchange(host, port, index);
                    warmup(exchange);
                    yield measure(exchange);
                }
            }
            case FILE_READ, FILE_WRITE -> {
                try (DemoFileStore store = new DemoFileStore(config.threads())) {
                    boolean read = config.transport() == Transport.FILE_READ;
                    Exchange exchange = index -> read
                            ? fileReadExchange(store.readPath(index))
                            : fileWriteExchange(store.writePath(index));
                    warmup(exchange);
                    yield measure(exchange);
                }
            }
        };
    }

    private void warmup(Exchange exchange) throws ExecutionException, InterruptedException {
        try (ExecutorService executor = Executors.newVirtualThreadPerTaskExecutor()) {
            List<Future<?>> futures = new ArrayList<>();
            for (int i = 0; i < config.threads(); i++) {
                int index = i;
                futures.add(executor.submit(() -> request(exchange, index)));
            }
            for (Future<?> future : futures) {
                future.get();
            }
        }
        System.out.printf("[warmup] %d tasks completed%n", config.threads());
    }

    private Stats measure(Exchange exchange) throws ExecutionException, InterruptedException {
        long ok = 0;
        long failed = 0;
        long min = Long.MAX_VALUE;
        long max = Long.MIN_VALUE;
        long total = 0;
        long tasks = 0;
        long wall = 0;
        long benchStart = System.nanoTime();
        long maxRounds = config.loop() ? Long.MAX_VALUE : config.rounds();

        for (long round = 1; round <= maxRounds; round++) {
            AtomicLong okRound = new AtomicLong();
            AtomicLong failedRound = new AtomicLong();
            AtomicLong minRound = new AtomicLong(Long.MAX_VALUE);
            AtomicLong maxRound = new AtomicLong(Long.MIN_VALUE);
            AtomicLong totalRound = new AtomicLong();

            long start = System.nanoTime();
            try (ExecutorService executor = Executors.newVirtualThreadPerTaskExecutor()) {
                List<Future<?>> futures = new ArrayList<>(config.threads());
                for (int i = 0; i < config.threads(); i++) {
                    int index = i;
                    futures.add(executor.submit(() -> {
                        long t0 = System.nanoTime();
                        try {
                            if (request(exchange, index)) {
                                okRound.incrementAndGet();
                            } else {
                                failedRound.incrementAndGet();
                            }
                        } catch (Throwable t) {
                            failedRound.incrementAndGet();
                        } finally {
                            long dt = System.nanoTime() - t0;
                            totalRound.addAndGet(dt);
                            minRound.accumulateAndGet(dt, Math::min);
                            maxRound.accumulateAndGet(dt, Math::max);
                        }
                    }));
                }
                for (Future<?> future : futures) {
                    future.get();
                }
            }
            wall = (System.nanoTime() - start) / 1_000_000;

            // 轮与轮之间固定间隔，避免上一轮的 wall 把本轮计时带偏
            if (round < maxRounds) {
                Thread.sleep(ROUND_INTERVAL.toMillis());
            }

            ok += okRound.get();
            failed += failedRound.get();
            tasks += config.threads();
            total += totalRound.get();
            min = Math.min(min, minRound.get());
            max = Math.max(max, maxRound.get());

            System.out.printf(
                    "[round %d] transport=%s mode=%s threads=%d wall=%dms ok=%d failed=%d task(avg/min/max)=%d/%d/%d us%n",
                    round, config.transport(), config.mode(), config.threads(), wall,
                    okRound.get(), failedRound.get(),
                    totalRound.get() / config.threads() / 1_000,
                    minRound.get() / 1_000,
                    maxRound.get() / 1_000);

            if (config.loop()) {
                long elapsedSec = Math.max(1, (System.nanoTime() - benchStart) / 1_000_000_000L);
                System.out.printf(
                        "         [cumulative] rounds=%d tasks=%d ok=%d failed=%d throughput=%d tasks/s avgTask=%d us%n",
                        round, tasks, ok, failed, tasks / elapsedSec,
                        tasks == 0 ? 0 : total / tasks / 1_000);
            }
        }

        long avg = tasks == 0 ? 0 : total / tasks;
        return new Stats(wall, tasks, ok, failed, min, max, avg);
    }

    private boolean request(Exchange exchange, int index) {
        try {
            boolean ok;
            if (config.mode() == Mode.PINNED) {
                synchronized (pinMonitors[index % pinMonitors.length]) {
                    ok = exchange.exchange(index);
                }
            } else {
                ok = exchange.exchange(index);
            }
            if (config.postSleep().toMillis() > 0) {
                Thread.sleep(config.postSleep().toMillis());
            }
            return ok;
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            return false;
        } catch (Exception e) {
            return false;
        }
    }

    private boolean httpExchange(String url, int index) throws IOException, InterruptedException {
        HttpRequest request = HttpRequest.newBuilder(URI.create(url))
                .timeout(Duration.ofSeconds(10))
                .GET()
                .build();
        HttpResponse<String> response = client.send(request, HttpResponse.BodyHandlers.ofString());
        return response.statusCode() == 200;
    }

    // 阻塞读：read() 跑在调用方的虚拟线程上 -> SocketChannelImpl 检测 isVirtual -> 走 Poller(POLLIN)
    private boolean socketReadExchange(String host, int port, int index) throws IOException {
        try (Socket socket = new Socket()) {
            socket.setTcpNoDelay(true);
            socket.connect(new InetSocketAddress(host, port), 5_000);
            socket.setSoTimeout(10_000);
            OutputStream out = socket.getOutputStream();
            out.write("GET /io\r\n\r\n".getBytes(StandardCharsets.US_ASCII));
            out.flush();
            InputStream in = socket.getInputStream();
            byte[] buffer = new byte[64];
            long total = 0;
            int read;
            while ((read = in.read(buffer)) != -1) { // 不断读直到读完（服务端发完即关闭产生 EOF）
                total += read;
            }
            return total > 0;
        }
    }

    // 阻塞写：小发送缓冲 + 大 payload + 服务端延迟读取 -> write() 阻塞在虚拟线程上 -> 走 Poller(POLLOUT)
    private boolean socketWriteExchange(String host, int port, int index) throws IOException {
        try (Socket socket = new Socket()) {
            socket.setTcpNoDelay(true);
            socket.setSendBufferSize(8 * 1024); // 调小，确保 write 会因缓冲区满而阻塞
            socket.connect(new InetSocketAddress(host, port), 5_000);
            socket.setSoTimeout(10_000);
            OutputStream out = socket.getOutputStream();
            byte[] payload = randomWritePayload(); // 交替随机长度，大块会因发送缓冲区满而阻塞
            out.write(payload);
            out.flush();
            socket.shutdownOutput(); // 发出 EOF，让服务端排空循环结束
            System.out.printf("[client] sent %d bytes%n", payload.length);
            return true;
        }
    }

    // 交替发送：奇数次 [1, 8K] 字节，偶数次 [8K, 512K] 字节，内容填充随机
    private byte[] randomWritePayload() {
        long n = writeCounter.incrementAndGet();
        int size = (n % 2 == 1)
                ? ThreadLocalRandom.current().nextInt(WRITE_SMALL_MIN, WRITE_SMALL_MAX + 1)
                : ThreadLocalRandom.current().nextInt(WRITE_LARGE_MIN, WRITE_LARGE_MAX + 1);
        return filled(size, (byte) 'x');
    }

    // 阻塞文件读：普通文件不是 SelectableChannel，无法注册到 Poller；
    // 当底层文件系统真的阻塞时，会 pin 住载体线程（tmpfs 上太快可能观测不到）
    private boolean fileReadExchange(Path path) throws IOException {
        try (InputStream in = new FileInputStream(path.toFile())) {
            byte[] buffer = new byte[64 * 1024];
            long total = 0;
            int read;
            while ((read = in.read(buffer)) != -1) {
                total += read;
            }
            return total > 0;
        }
    }

    // 阻塞文件写：write + fsync 都是不可 poll 的阻塞 IO，无法走 Poller，真阻塞时 pin 载体线程
    private boolean fileWriteExchange(Path path) throws IOException {
        try (FileOutputStream out = new FileOutputStream(path.toFile())) {
            out.write(FILE_WRITE_PAYLOAD);
            out.flush();
            out.getFD().sync(); // fsync：真实阻塞，演示文件 IO 的 pin 行为
            return true;
        }
    }

    private static byte[] filled(int size, byte value) {
        byte[] buffer = new byte[size];
        Arrays.fill(buffer, value);
        return buffer;
    }
}
