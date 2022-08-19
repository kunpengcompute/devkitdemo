# **Gzip demo**

简体中文 | [English](README_en.md)

## 介绍
1. [Gzip](https://github.com/kunpengcompute/gzip)（GNU zip）是一款发布较早并已广泛应用的压缩软件。其优化版本在官网发布的Gzip-1.10 Release版本基础上，通过数据预取、循环展开、CRC指令替换等方法，来提升其在鲲鹏计算平台上的压缩和解压缩速率，尤其对文本类型文件的压缩及解压具有更明显的性能优势。

## Gzip编译依赖
1. 确保先按照 **Gzip**;
[Gzip下载地址](https://github.com/kunpengcompute/gzip)
[编译参考](https://github.com/kunpengcompute/gzip)
2. 或者安装**kunpeng-gpc**和**kunpeng-gpc-devel** SDK

## 使用教程

1. 运行gzip
   ```shell
    /usr/local/bin/gzip -h
   ```

