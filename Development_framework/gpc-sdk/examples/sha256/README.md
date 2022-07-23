# **sha256 demo**

简体中文 | [English](README_en.md)

## SHA256简介

SHA256是SHA-2下细分出的一种算法。

SHA-2，名称来自于安全散列算法2（英语：Secure Hash Algorithm 2）的缩写，一种密码散列函数算法标准，由美国国家安全局研发，属于SHA算法之一，是SHA-1的后继者。

SHA-2下又可再分为六个不同的算法标准，包括：SHA-224、SHA-256、SHA-384、SHA-512、SHA-512/224、SHA-512/256。这些变体除了生成摘要的长度 、循环运行的次数等一些微小差异外，算法的基本结构是一致的。

SHA256，本质上就是一个哈希函数。
哈希函数，又称散列算法，是一种从任何一种数据中创建小的数字“指纹”的方法。散列函数把消息或数据压缩成摘要，使得数据量变小，将数据的格式固定下来。该函数将数据打乱混合，重新创建一个叫做散列值（或哈希值）的指纹。散列值通常用一个短的随机字母和数字组成的字符串来代表。

对于任意长度的消息，SHA256都会产生一个256bit长的哈希值，称作消息摘要。这个摘要相当于是个长度为32个字节的数组，通常用一个长度为64的十六进制字符串来表示

## 使用教程

1. 获取代码

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. 切入到项目根路径

   ```shell
   cd ./devkitdemo/Development_framework/gpc-sdk/examples/sha256/
   ```

3. 编译demo

   ```shell
   mkdir build
   cd build
   cmake ..
   make
   ```

4. 运行demo
   ```shell
   ./sha256 ../src/sha256.c
   ```
   说明：第一个参数为要执行的二进制文件，第二个参数为用于测试的数据文件，可以为任意文件，本例以src/sha256.c文件为例；
   注意：为了明显对比用C实现的sha256算法和用汇编实现的sha256算法的性能，建议测试文件大小为1M以上。

5. 清理demo

   ```shell
   cd ..
   rm -rf build
   ```