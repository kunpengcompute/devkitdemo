# **Kunpeng HPC SDK One-Click Installation Script**

English | [简体中文](README.md)

## 1、Introduction

#### 1、Use the automatic installation script to automatically deploy HPC software, including HYPER-MPI, KML, GCC, and BISHENG.

#### 2、Support installation in a user-defined path.


## 2、Dependency

#### 1、The server compatibility is as follows:

|  OS  | Hyper MPI  | BISHENG | GCC for openEuler  | KML   |  
|  ---- | ----  | ---- | ---- | ----  |
| centos 7  | Y (GCC 9.3,BISHENG 2.1.0) | Y | Y | Y (GCC 7.3 or later) |
| openEuler20.03 LTS  | Y (BISHNEG 2.1.0) | Y | Y | Y (GCC 7.3 or later) |
| openEuler20.03 sp1  | Y (GCC 9.3,BISHENG 2.1.0) | Y | Y | Y (GCC 7.3 or later) |
| openEuler20.03 sp2  | N | Y | Y | Y (GCC 7.3 or later) |
| openEuler20.03 sp3  | N | Y | Y | Y (GCC 7.3 or later) |
| kylin V10 SP1  | Y (GCC 9.3,BISHENG 2.1.0) | Y | Y | N  |
| kylin V10 SP2  | Y (GCC 9.3,BISHENG 2.1.0) | Y | Y | N  |
| unbuntu18.04  | N | Y | Y | Y (GCC 7.3 or later)| 
| unbuntu20.04  | N | Y | Y | Y (GCC 7.3 or later) |
| UOS 1020e | N | Y | Y | N  | 

Note: The OSs listed in the preceding table use the AArch64 architecture. The versions of the GCC and Bisheng Compiler in parentheses indicate the earliest supported versions. Y indicates supported installation and N indicates the installation is not supported.

#### 2、Software dependency list


|  Dependency  | Hyper MPI  | BISHENG | GCC for openEuler  | KML   |  
|  ---- | ----  | ---- | ---- | ----  |
| gcc  | Y(GCC 9.3.0) | Y(GCC 4.8.5 or later)| N | N |
| bisheng  | Y | N | N | N  |
| glibc-devel(libc-dev-bin) | N  | N | Y(glibc-devel 2.17 or later) | N  |
| glibc  | N | Y(glibc 2.17 or later)| Y(glibc 2.17 or later) | N  |
| libatomic  | N | Y(libatomic 1.2 or later) | N | N  |
| libgomp  | N | N | N | Y  | 

Note: The preceding table lists the dependencies required for installing the Hyper MPI, math library, GCC for openEuler, and Bisheng Compiler. Y indicates required dependency for the software and N indicates the dependency is not required. Before installing the RPM　package compiled by Bisheng in the Red Hat system, ensure that libgomp has been installed in the system.

## 3、Usage Guide

####  1、Obtain the kunpeng-hpc-1.0.0-aarc64-linux.tar.gz package. 
[https://mirrors.huaweicloud.com/kunpeng/archive/Kunpeng_SDK/HPC/kunpeng-hpc-1.0.0-aarch64-linux.tar.gz]
(https://mirrors.huaweicloud.com/kunpeng/archive/Kunpeng_SDK/HPC/kunpeng-hpc-1.0.0-aarch64-linux.tar.gz)
####  2、Run the following commands to install the software:
```
tar -xf kunpeng-hpc-xxx-aarch64-linux.tar.gz
cd kunpeng-hpc-xxx-aarch64-linux/script
bash install.sh
```
#### 3、Perform operations as prompted to install the HPC software. 
#### 4、After the installation is complete, add environment variables ad prompted. The following command output uses GCC as an example:

>  4.1 Method 1: Add environment variables to /etc/profile.
```
  echo 'export PATH=/opt/gcc/gcc-xxx-aarch64-linux/bin:$PATH' >>/etc/profile
  echo 'export INCLUDE=/opt/gcc/gcc-xxx-aarch64-linux/include:$INCLUDE' >>/etc/profile
  echo 'export LD_LIBRARY_PATH=/opt/gcc/gcc-xxx-aarch64-linux/lib64:$LD_LIBRARY_PATH' >>/etc/profile
  source /etc/profile
```
>  4.2 Method 2: Use the module command to load module_file for the environment variables to take effect.
```
  module load /opt/modules/gcc/gcc_modulefiles
```
> 4.3 Method 3: Make the environment variables take effect permanently.

> * 1、Open the ~/.bashrc.
```
  vim ~/.bashrc  
```
> * 2、Add the following content to the ~/.bashrc file:
```
  module use /opt/modules/gcc
  module load gcc_modulefiles
```
> * 3、Save the file.
> * 4、Make the configuration take effect:
```
  source ~/.bashrc
```
4.4 Method 4: Run the script to configure environment variables.
```
  cd kunpeng-hpc-xxx-aarch64-linux/script
  bash configure_environment_xxxxx.sh
```
## 4、Precautions

#### 1、The free drive space for the installation must be greater than 3 GB.
#### 2、The installation can be performed only in the AArch64 environment.

## 5、Software Uninstallation
#### The following uses gcc and kml as an example.
#### 1、Open /etc/profile or ~/.bashrc to check the directory where the binary or lib files of related software are stored.
```
vim /etc/profile（~/.bashrc）
export LD_LIBRARY_PATH=/usr/local/kml/lib:$LD_LIBRARY_PATH
...
export LD_LIBRARY_PATH=/usr/local/kml/lib/kspblas/multi:$LD_LIBRARY_PATH
export INCLUDE=/opt/gcc/gcc-xxxx-aarch64-linux/include:$INCLUDE
```
Note: After you access /etc/profile (~/.bashrc), you can view that the installation paths are /usr/local/kml and /opt/gcc/gcc-xxxx-aarch64-linux/ or Run the 'env / export' command to view the software installation path.

#### 2、Delete environment variables.
#### 3、In the installation path obtained in  step 1, ensure that there are no other files to be stored in the path, and then delete the installation path. In then path, '/opt' is then default installation path. Replace it if it is a user-defined path.
#### 4、Open a new terminal window for the modified environment variables to take effect.
