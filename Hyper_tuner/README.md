# Hyper Tuner demo
鲲鹏Hyper Tuner系统性能优化工具是针对TaiShan服务器的性能分析和优化工具，能收集服务器的处理器硬件、操作系统、进程/线程、函数等各个层次的性能数据，分析出系统性能指标，定位到系统瓶颈及热点函数，并给出相应解决问题的优化建议。
这个demo展示了如何使用鲲鹏Hyper Tuner系统性能优化工具中的各个功能模块发现系统或应用中存在的问题，解决系统瓶颈并对代码进行优化。
\
\
testdemo文件夹包含cache, falsesharing, lock, multiply, test_swap.c文件。
- cache \
  该文件夹中包含cache_miss(优化前的示例代码)和cache_hit(优化后的示例代码)，是对列访问循环工具检测优化的示例。
- falsesharing \
  该文件夹包含falsesharing.c(优化前的示例代码)和falsesharing_mod.c(优化后的示例代码)，是对伪共享分析的示例。
- lock \
  该文件夹包含pthread_mutex，是对所频繁抢占的示例；pthread_mutex_long.c(优化前的示例代码)和pthread_mutex_long_mod.c(优化后的示例代码)，是对锁长期等待的示例。
- multiply \
  该文件夹包含multiply.c(优化前的示例代码)和multiply_simd.c(优化后的示例代码)，是对矩阵分析并用neon优化的示例。
- test_swap.c \
  该文件是触发SWAP优化建议的示例。
\
\
具体使用流程说明书详见华为云鲲鹏开发套件-鲲鹏性能分析工具 https://support.huaweicloud.com/kunpengdevps/kunpengdevps.html
