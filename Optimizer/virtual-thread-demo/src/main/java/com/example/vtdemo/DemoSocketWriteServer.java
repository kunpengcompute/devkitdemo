package com.example.vtdemo;

import java.io.IOException;
import java.io.InputStream;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.time.Duration;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * 用于 socket 写场景：服务端先 sleep(latency) 再排空客户端数据，
 * 从而让客户端的 write() 因发送缓冲区满而阻塞约 latency 毫秒（走 Poller POLLOUT）。
 */
public final class DemoSocketWriteServer implements AutoCloseable {

    private final ServerSocket server;
    private final Duration latency;
    private final ExecutorService executor;

    public DemoSocketWriteServer(Duration latency) throws IOException {
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
            // 延迟排空，迫使客户端的阻塞写 park 约 latency 后再被逐步读走
            try {
                Thread.sleep(latency.toMillis());
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                return;
            }
            InputStream in = socket.getInputStream();
            byte[] buffer = new byte[64 * 1024];
            while (in.read(buffer) != -1) {
                // 一直读到客户端 shutdownOutput 产生的 EOF
            }
        } catch (IOException e) {
            // client went away, ignore
        }
    }

    @Override
    public void close() throws IOException {
        server.close();
        executor.shutdown();
    }
}
