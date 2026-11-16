package com.example.vtdemo;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.time.Duration;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.ThreadLocalRandom;
import java.util.concurrent.atomic.AtomicLong;

public final class DemoSocketServer implements AutoCloseable {

    // 交替发送：奇数次响应 [1,63] 字节，偶数次响应 [200,1024] 字节
    private static final int SMALL_MIN = 1;
    private static final int SMALL_MAX = 63;
    private static final int LARGE_MIN = 200;
    private static final int LARGE_MAX = 1024;

    private final AtomicLong responseCount = new AtomicLong();

    private final ServerSocket server;
    private final Duration latency;
    private final ExecutorService executor;

    public DemoSocketServer(Duration latency) throws IOException {
        this.latency = latency;
        this.executor = Executors.newCachedThreadPool();
        this.server = new ServerSocket();
        this.server.bind(new InetSocketAddress("127.0.0.1", 0));
    }

    public void start() {
        executor.submit(this::acceptLoop);
    }

    public String host() {
        return server.getInetAddress().getHostAddress();
    }

    public int port() {
        return server.getLocalPort();
    }

    private void acceptLoop() {
        while (!server.isClosed()) {
            try {
                Socket socket = server.accept();
                executor.submit(() -> handle(socket));
            } catch (IOException e) {
                if (server.isClosed()) {
                    return;
                }
            }
        }
    }

    private void handle(Socket socket) {
        try (socket) {
            socket.setTcpNoDelay(true);
            InputStream in = socket.getInputStream();
            byte[] buffer = new byte[256];
            in.read(buffer);
            try {
                Thread.sleep(latency.toMillis());
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                return;
            }
            OutputStream out = socket.getOutputStream();
            byte[] payload = randomPayload();
            out.write(payload);
            out.flush();
            System.out.printf("[server] sent %d bytes%n", payload.length);
        } catch (IOException e) {
            // client went away, ignore
        }
    }

    // 交替发送：奇数次 [1,128] 字节，偶数次 [200,1024] 字节，内容随机
    private byte[] randomPayload() {
        long n = responseCount.incrementAndGet();
        int size = (n % 2 == 1)
                ? ThreadLocalRandom.current().nextInt(SMALL_MIN, SMALL_MAX + 1)
                : ThreadLocalRandom.current().nextInt(LARGE_MIN, LARGE_MAX + 1);
        byte[] payload = new byte[size];
        ThreadLocalRandom.current().nextBytes(payload);
        return payload;
    }

    @Override
    public void close() throws IOException {
        server.close();
        executor.shutdown();
    }
}
