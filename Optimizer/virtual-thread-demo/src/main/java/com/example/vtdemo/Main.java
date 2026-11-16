package com.example.vtdemo;

import java.time.Duration;

public final class Main {

    public static void main(String[] args) throws Exception {
        if (contains(args, "--help") || contains(args, "-h")) {
            printUsage();
            return;
        }

        int threads = intArg(args, "--threads", 200);
        int rounds = intArg(args, "--rounds", 3);
        long latencyMs = longArg(args, "--latency", 200);
        long postSleepMs = longArg(args, "--post-sleep", 50);
        VirtualThreadIoDemo.Mode mode = "pinned".equalsIgnoreCase(strArg(args, "--mode", "normal"))
                ? VirtualThreadIoDemo.Mode.PINNED
                : VirtualThreadIoDemo.Mode.NORMAL;
        VirtualThreadIoDemo.Transport transport = parseTransport(strArg(args, "--scenario", "http"));
        boolean loop = contains(args, "--loop") || contains(args, "--forever");

        VirtualThreadIoDemo.Config config = new VirtualThreadIoDemo.Config(
                threads, rounds, Duration.ofMillis(latencyMs), Duration.ofMillis(postSleepMs), mode, transport, loop);

        System.out.printf("config: scenario=%s threads=%d rounds=%d latency=%dms postSleep=%dms mode=%s loop=%b%n",
                transport.name().toLowerCase(), threads, rounds, latencyMs, postSleepMs, mode, loop);
        System.out.printf("java: %s, virtualThreadsSupported=%b%n",
                System.getProperty("java.version"), true);

        VirtualThreadIoDemo demo = new VirtualThreadIoDemo(config);
        VirtualThreadIoDemo.Stats stats = demo.run();

        System.out.printf("%nsummary: tasks=%d ok=%d failed=%d task(avg/min/max)=%d/%d/%d us%n",
                stats.tasks(), stats.ok(), stats.failed(),
                stats.avgTaskNanos() / 1_000, stats.minTaskNanos() / 1_000, stats.maxTaskNanos() / 1_000);
    }

    private static VirtualThreadIoDemo.Transport parseTransport(String value) {
        String v = value.toLowerCase().replace("-", "").replace("_", "");
        return switch (v) {
            case "http" -> VirtualThreadIoDemo.Transport.HTTP;
            case "socket", "socketread", "read" -> VirtualThreadIoDemo.Transport.SOCKET_READ;
            case "socketwrite", "write" -> VirtualThreadIoDemo.Transport.SOCKET_WRITE;
            case "fileread" -> VirtualThreadIoDemo.Transport.FILE_READ;
            case "filewrite" -> VirtualThreadIoDemo.Transport.FILE_WRITE;
            default -> throw new IllegalArgumentException("unknown --scenario: " + value);
        };
    }

    private static boolean contains(String[] args, String key) {
        for (String arg : args) {
            if (arg.equals(key)) {
                return true;
            }
        }
        return false;
    }

    private static String strArg(String[] args, String key, String def) {
        for (int i = 0; i < args.length - 1; i++) {
            if (args[i].equals(key)) {
                return args[i + 1];
            }
        }
        return def;
    }

    private static int intArg(String[] args, String key, int def) {
        return Integer.parseInt(strArg(args, key, String.valueOf(def)));
    }

    private static long longArg(String[] args, String key, long def) {
        return Long.parseLong(strArg(args, key, String.valueOf(def)));
    }

    private static void printUsage() {
        System.out.println("""
                Usage: java -jar vtdemo.jar [options]
                  --threads <n>      number of virtual threads per round (default 200)
                  --rounds <n>       number of measured rounds (default 3)
                  --latency <ms>     server-side IO latency per request (default 200)
                  --post-sleep <ms>  client-side sleep after each request (default 50)
                  --mode <normal|pinned>  pin carrier threads with synchronized (default normal)
                  --scenario <name>  IO path, pick exactly one case to run:
                        http          java.net.http.HttpClient (blocking read/write on HttpClient's platform worker threads)
                        socket        (alias: read) blocking Socket read on the caller's virtual thread -> Poller POLLIN
                        socketwrite   (alias: write) blocking Socket write on the caller's virtual thread -> Poller POLLOUT
                        fileread      blocking FileInputStream read -> not pollable -> pins the carrier
                        filewrite     blocking FileOutputStream + fsync -> not pollable -> pins the carrier
                                    (default: http)
                  --loop, --forever  run rounds forever (never exits, ~每轮打印累计吞吐), ignore --rounds
                  -h, --help         show this help
                """);
    }
}
