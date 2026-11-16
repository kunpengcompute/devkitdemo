package com.example.vtdemo;

import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpServer;

import java.io.IOException;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.nio.charset.StandardCharsets;
import java.time.Duration;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public final class DemoHttpServer implements AutoCloseable {

    private final HttpServer server;
    private final Duration latency;
    private final ExecutorService executor;

    public DemoHttpServer(Duration latency) throws IOException {
        this.latency = latency;
        this.executor = Executors.newCachedThreadPool();
        this.server = HttpServer.create(new InetSocketAddress("127.0.0.1", 0), 0);
        this.server.setExecutor(executor);
        this.server.createContext("/io", this::handle);
        this.server.createContext("/fast", this::handleFast);
    }

    public void start() {
        server.start();
    }

    public int port() {
        return server.getAddress().getPort();
    }

    public String baseUrl() {
        return "http://127.0.0.1:" + port();
    }

    private void handle(HttpExchange exchange) throws IOException {
        try {
            Thread.sleep(latency.toMillis());
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
        respond(exchange, "ok");
    }

    private void handleFast(HttpExchange exchange) throws IOException {
        respond(exchange, "ok");
    }

    private void respond(HttpExchange exchange, String body) throws IOException {
        byte[] payload = body.getBytes(StandardCharsets.UTF_8);
        exchange.getResponseHeaders().set("Content-Type", "text/plain");
        exchange.sendResponseHeaders(200, payload.length);
        try (OutputStream out = exchange.getResponseBody()) {
            out.write(payload);
        }
    }

    @Override
    public void close() {
        server.stop(0);
        executor.shutdown();
    }
}
