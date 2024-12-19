# **Hcom Demo**

English | [简体中文](README.md)

## Dependency

The following minimum requirements should be guaranteed.

- CPU: Huawei Kunpeng 920 Processor
- Server: TaiShan 200 Server (model 2280)
- Operating System: OpenEuler 20.03 LTS SP1, OpenEuler 22.03 LTS SP1, CentOS 7.6.18.10
- cmake2.18.2.
- hcom Software

## Software Installation

If the hcom package is not installed, please follow these steps。

Step 1: Obtain the hcom package and place it in /your_path/to/hcom.

Step 2: Install the RPM package for hcom.

```sh
$ rpm -ivh /your_path/to/hcom/kunpeng-hcom-1.0.0-1.aarch64.rpm
$ rpm -ivh /your_path/to/hcom/kunpeng-hcom-devel-1.0.0-1.aarch64.rpm
```

Step 3: Verify the RPM installation status.
```sh
$ rpm -qi kunpeng-hcom-1.0.0-1.aarch64.rpm
$ rpm -qi kunpeng-hcom-devel-1.0.0-1.aarch64.rpm
```
The installation is successful if information similar to the following appears.
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

## Usage Guidance
Step 1. Download Codes (Assuming at /home directory).
```sh
$ git clone https://github.com/kunpengcompute/devkitdemo.git
```

Step 2. Change to Demo Directory.
```sh
$ cd /home/devkitdemo/hcom-sdk/examples/hcom
```

Step 3. Compile Demo.
```sh
$ rm -rf build
$ mkdir -p build

$ cd build
$ cmake ..
$ make
$ cd ..
```

Step 4: Start the server application
```sh
$ cd build
$ ./server -i {ip} -p {port} -c asyncCpuId -e 0 -r 0 -s 4 -b 0 -d 1024
```
Parameter Description.

| Parameter          | Description                               |
|---------------|------------------------------------|
| `-i`          | Server IP, e.g., 10.175.118.1 |
| `-p`          | Server port, e.g., 9981          |
| `-c`          | Asynchronous worker                        |
| `-e`          | Enable TLS                           |
| `-r`          | Send raw data                       |
| `-s`          | Buffer size                         |
| `-b`          | Blocking IO                           |
| `-d`          | Data size                           |



Step 5: Start the client application
```sh
$ ./client -i {ip} -p {port} -t 1000000 -c 15 -b 0 -w 0 -e 0 -r 0 -s 4 -d 1024
```
Parameter Description.

| Parameter          | Description                               |
|---------------|------------------------------------|
| `-i`          | Server IP, e.g., 10.175.118.1 |
| `-p`          | Server port, e.g., 9981          |
| `-t`          | Number of communication attempts                    |
| `-c`          | Asynchronous worker                        |
| `-b`          | Blocking IO                           |
| `-w`          | Called by worker                     |
| `-e`          | Enable TLS                           |
| `-r`          | Send raw data                       |
| `-s`          | Buffer size                         |
| `-d`          | Data size                           |

**Expected Results.**：
1、The server starts successfully.
2、The client successfully connects to the server.
3、The client terminal displays the time consumption for the specified number of communication attempts.


Step 6. Clean Demo.
```sh
$ rm -rf build
```

