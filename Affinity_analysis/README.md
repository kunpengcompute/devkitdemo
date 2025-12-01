# Affinity Analysis demo
亲和分析工具是针对鲲鹏平台的开发者、用户和ISV开发者的应用、源码分析工具。该工具属于鲲鹏DevKit的扩展工具之一，支持软件代码质量检查，提升代码质量、优化访存性能等。
\
\
testdemo文件夹包含precision和knet文件夹。
- precision \
  该文件夹中包含C和fortran语言的demo文件，是精度分析工具的示例。
- knet \
  该文件夹包含test_posix.c(示例代码)和config.txt(配置文件)，是对链路时延检测工具的示例。
\
\
其他工具的使用示例见课程 https://www.hikunpeng.com/learn/courses-list/detail/1981972282475433986

包括:
1. 64位运行模式检查
2. 字节对齐检查
3. BC文件生成
4. 内存一致性静态检查
5. 向量化检查
6. 矩阵化检查
7. 构建亲和
8. 缓存行对齐检查
9. 内存一致性动态检查

工具详细介绍、具体使用流程说明和FAQ等详见鲲鹏社区用户指南:
\
亲和分析工具 IDE插件：https://www.hikunpeng.com/document/detail/zh/kunpengdevps/userguide/Plugins_UserGuide/DevKit_Affinity_0001.html
\
亲和分析工具 命令行：https://www.hikunpeng.com/document/detail/zh/kunpengdevps/userguide/cliuserguide/KunpengDevKitCli_0018.html
