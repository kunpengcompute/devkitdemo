package com.example.vtdemo;

import java.io.IOException;
import java.io.UncheckedIOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Arrays;
import java.util.Comparator;
import java.util.stream.Stream;

/**
 * 文件读写场景的临时文件仓库：
 *   - 读取场景预先创建 threads 个文件；
 *   - 写入场景提供 per-task 的目标路径；
 *   - close() 递归删除整个临时目录。
 */
public final class DemoFileStore implements AutoCloseable {

    private static final int READ_SIZE = 256 * 1024;

    private final Path dir;

    public DemoFileStore(int files) throws IOException {
        this.dir = Files.createTempDirectory("vtdemo-file-");
        byte[] content = new byte[READ_SIZE];
        Arrays.fill(content, (byte) 'r');
        for (int i = 0; i < files; i++) {
            Files.write(readPath(i), content);
        }
    }

    public Path readPath(int index) {
        return dir.resolve("read-" + index + ".bin");
    }

    public Path writePath(int index) {
        return dir.resolve("write-" + index + ".tmp");
    }

    @Override
    public void close() throws IOException {
        try (Stream<Path> walk = Files.walk(dir)) {
            walk.sorted(Comparator.reverseOrder()).forEach(p -> {
                try {
                    Files.deleteIfExists(p);
                } catch (IOException e) {
                    throw new UncheckedIOException(e);
                }
            });
        }
    }
}
