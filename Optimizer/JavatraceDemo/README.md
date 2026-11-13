# JavatraceDemo —— Java / JNI 性能问题可观测性样例

一个可被 `perf` / `async-profiler` / `bpf` 等 trace 工具量化的 Java + JNI 订单流水线 demo，
故意制造三类典型性能问题，并提供优化对照版用于验证 trace 工具的有效性。

---

## 1. 项目目标

演示 **三类可被 trace 工具捕捉的性能问题**：

| #   | 问题                  | 表现                                                     | 对应的 trace 信号                                                            |
| --- | --------------------- | -------------------------------------------------------- | ---------------------------------------------------------------------------- |
| 1   | calcPrice 纯计算热点  | 单订单 CPU-bound 800 轮哈希 / xorshift                   | `perf top` 出现 `Java_NativeBridge_calcPrice`，async-profiler CPU 火焰图顶部 |
| 2   | checkStock 全局锁竞争 | 重计算 + 原子递减 + 16 次小 write 全部放进 pthread_mutex | `futex` 等待、`pthread_mutex_lock` 占比高，线程在 futex 上排队               |
| 3   | asyncPersist p99 长尾 | 每 64 笔订单 `usleep(1500)`                              | async-profiler wall-clock 采样显示 `usleep` / `nanosleep` 在长尾路径         |

优化对照版 (`order_pipline_good.cpp`) 把锁内的重计算和 IO 移到锁外、去掉人为 usleep，
用于对比 **futex 等待** 和 **p99 长尾** 的下降。

---

## 2. 目录结构

```
JavatraceDemo/
├── Makefile                     # all / bad / good / clean / run / run-bad / run-good
├── NativeBridge.java            # 3 个 static native 声明（calcPrice / checkStock / asyncPersist）
├── OrderPipelineDemo.java       # main + CyclicBarrier + N 个 order-worker-i 循环跑
├── order_pipline_bad.cpp        # 问题版 .so：CPU 热点 + 锁竞争 + p99 长尾
├── order_pipline_good.cpp       # 优化版 .so：锁外计算 + 锁内最小化 IO + 无 usleep
└── README.md
```

---

## 3. 业务流程

```
每笔订单（每 worker 私有 1KB byte[]）：

  Java 侧校验 ──► JNI calcPrice ──► JNI checkStock ──► JNI asyncPersist
                  （CPU 热点）       （锁竞争）         （p99 长尾）
                                            ▲
                                     atomic stock = 1,000,000,000
                                     初值 10 亿，每次递减 1
```

- **Java 侧**：`main` 接收两个可选参数 —— 运行秒数（默认 60）、worker 线程数（默认 4）；
  打印时长、线程数、JVM PID；用 `CyclicBarrier` 等全部 worker 就绪后主线程开始计时；
  计时结束置停止标志、join 所有线程；每个 worker 退出时打印订单数和累计值。
- **JNI 侧**：全部通过 `GetPrimitiveArrayCritical` / `ReleasePrimitiveArrayCritical` 拿数组，
  避免拷贝开销；内部计算/写函数全部 `__attribute__((noinline))`，JVM 编译参数也关闭内联。

---

## 4. 编译与运行

### 4.1 环境要求

- Linux x86_64（perf / async-profiler / bpf 都依赖）
- g++（支持 C++11 原子操作）
- JDK 8 / 11 / 17 / 21（JDK 6+ 可选，自动探测）
- `make`

### 4.2 Makefile 目标

| 目标                             | 作用                                |
| -------------------------------- | ----------------------------------- |
| `make all`                       | 默认，等价于 `make good`            |
| `make bad`                       | 编译 bad 版 `liborder_pipeline.so`  |
| `make good`                      | 编译 good 版 `liborder_pipeline.so` |
| `make run-bad` / `make run-good` | 切换后运行 60 秒 / 4 worker         |
| `make run`                       | 用当前已存在的 .so 运行             |
| `make clean`                     | 删除所有产物                        |

### 4.3 编译参数（关键）

Makefile 已经把优化/内联全部关闭，确保 trace 栈不被编译器吃掉：

```
-g -O0 -fPIC -fno-inline -fno-omit-frame-pointer \
-fno-optimize-sibling-calls -pthread -shared
```

JNI 库函数和内部计算/写函数全部加了 `__attribute__((noinline))`。

### 4.4 运行

**必须在 class 和 .so 所在目录运行**：

```bash
# 跑问题版（建议先用这个采集）
make run-bad

# 对照版
make run-good

# 或手动
make bad
java -Djava.library.path=. OrderPipelineDemo 60 8
```

输出示例：

```
=== OrderPipelineDemo ===
Duration(seconds) : 60
Worker threads    : 4
JVM PID           : 12345
All workers ready, start pipeline...
Stop signal sent, joining workers...
[order-worker-0] orders=4521  accumulated=123456789
[order-worker-1] orders=4498  accumulated=122890123
[order-worker-2] orders=4510  accumulated=123109876
[order-worker-3] orders=4505  accumulated=122998765
=== Done ===
Total orders      : 18034
Total accumulated : 492455553
Run finished.
```

**切换 bad/good 必须重新编译 .so**——Java 类本身不需要改。

---


