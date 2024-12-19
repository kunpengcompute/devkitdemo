# **Hcom Demo**

简体中文 | [English](README_en.md)

## 使用依赖

用户需要满足以下最低约束:

- CPU: Kunpeng 920处理器
- 服务器: TaiShan 200服务器 (型号2280)
- 操作系统: OpenEuler 20.03 LTS SP1, OpenEuler 22.03 LTS SP1, CentOS 7.6.18.10
- cmake2.18.2.
- hcom软件包

## 软件安装

如没有安装hcom软件包, 请依照以下步骤安装:

步骤1. 获取hcom软件包，放置于/your_path/to/hcom

步骤2. 安装hcom的RPM包。

```sh
$ rpm -ivh /your_path/to/hcom/kunpeng-hcom-1.0.0-1.aarch64.rpm
$ rpm -ivh /your_path/to/hcom/kunpeng-hcom-devel-1.0.0-1.aarch64.rpm
```

步骤3. 确认RPM安装情况。
```sh
$ rpm -qi kunpeng-hcom
$ rpm -qi kunpeng-hcom-devel
```
显示如下类似信息证明安装成功:
```
Name        : kunpeng-hcom
Version     : 1.0.0
Release     : 1
Architecture: aarch64
Install Date: xxxxxxxxxxxxxxxxx
Group       : Unspecified
Size        : 19113730
License     : Proprietary
Signature   : (none)
Source RPM  : kunpeng-hcom-1.0.0-1.src.rpm
Build Date  : Tue 05 Nov 2024 06:50:36 PM CST
Build Host  : ef2067181c1f
Summary     : kunpeng hcom
Description :
Kunpeng hcom for Secure Computing Application Development. Support os: xxxxxxxxxxxxx.


Name        : kunpeng-hcom-devel
Version     : 1.0.0
Release     : 1
Architecture: aarch64
Install Date: xxxxxxxxxxxxxxxxx
Group       : Unspecified
Size        : 19113730
License     : Proprietary
Signature   : (none)
Source RPM  : kunpeng-hcom-1.0.0-1.src.rpm
Build Date  : Tue 05 Nov 2024 06:50:36 PM CST
Build Host  : ef2067181c1f
Summary     : kunpeng hcom devel
Description :
Kunpeng hcom devel for Secure Computing Application Development. Support os: xxxxxxxxxxxxx.

```

## 使用教程
步骤1. 获取代码。 (假设当前在/home目录下)
```sh
$ git clone https://github.com/kunpengcompute/devkitdemo.git
```

步骤2. 切入到Demo根路径。
```sh
$ cd /home/devkitdemo/hcom-sdk/examples/hcom
```

步骤3. 编译Demo。
```sh
$ rm -rf build
$ mkdir -p build

$ cd build
$ cmake ..
$ make
$ cd ..
```

步骤4. 启动服务端应用程序
```sh
$ cd build
$ ./server -i {ip} -p {port} -c asyncCpuId -e 0 -r 0 -s 4 -b 0 -d 1024
```
参数说明：

| 参数          | 说明                               |
|---------------|------------------------------------|
| `-i`          | 服务端 IP，例如 10.175.118.1 |
| `-p`          | 服务端的端口，例如 9981          |
| `-c`          | 异步 worker                        |
| `-e`          | 启用 TLS                           |
| `-r`          | 发送原始数据                       |
| `-s`          | 缓冲区大小                         |
| `-b`          | 阻塞 IO                            |
| `-d`          | 数据大小                           |



步骤5. 启动客户端应用程序
```sh
$ ./client -i {ip} -p {port} -t 1000000 -c 15 -b 0 -w 0 -e 0 -r 0 -s 4 -d 1024
```
参数说明：

| 参数          | 说明                               |
|---------------|------------------------------------|
| `-i`          | 服务端 IP，例如 10.175.118.1 |
| `-p`          | 服务端的端口，例如 9981          |
| `-t`          | 通信次数                     |
| `-c`          | 异步 worker                        |
| `-b`          | 阻塞 IO                            |
| `-w`          | 由 worker 调用                     |
| `-e`          | 启用 TLS                           |
| `-r`          | 发送原始数据                       |
| `-s`          | 缓冲区大小                         |
| `-d`          | 数据大小                           |

**预期结果**：
1、服务端启动成功
2、客户端成功连接到服务端
3、客户端终端打印通信指定次数的时间消耗

步骤6. 清理Demo。
```sh
$ rm -rf build
```

