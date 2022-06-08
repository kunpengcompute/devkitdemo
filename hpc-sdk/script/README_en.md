# **Kunpeng HPC SDK One-Click Installation Script**

English | [简体中文](README_cn.md)

## 1、Introduction

#### 1、Use the automatic installation script to automatically deploy HPC software, including Hyper-MPI, KML, GCC, and Bisheng.

#### 2、Support installation in a user-defined path.


## 2、Dependency

#### 1、The server compatibility is as follows:

|  OS  | hyper-mpi  | BiSheng 2.1 | gcc  | KML   |  
|  ---- | ----  | ---- | ---- | ----  |
| centos 7  | Y (GCC >= 9.3,BISHENG) | Y | Y | Y (GCC >= 7.3) |
| openEuler20.03 LTS  | Y (BISHNEG) | Y | Y | Y (GCC >= 7.3) |
| openEuler20.03 sp1  | N | Y | Y | Y (GCC >= 7.3) |
| openEuler20.03 sp2  | N | Y | Y | Y (GCC >= 7.3) |
| openEuler20.03 sp3  | N | Y | Y | Y (GCC >= 7.3) |
| kylin V10 SP1  | Y (GCC >= 9.3,BISHENG) | Y | Y | N  |
| kylin V10 SP2  | Y (GCC >= 9.3,BISHENG) | Y | Y | N  |
| unbuntu18.04  | N | Y | Y | Y (GCC >= 7.3)| 
| unbuntu20.04  | N | Y | Y | Y (GCC >= 7.3) |
| UOS 1020e | N | Y | Y | N  | 

Note: The OSs listed in the preceding table use the AArch64 architecture. The versions of the GCC and Bisheng Compiler in parentheses indicate the earliest supported versions. Y indicates supported installation and N indicates the installation is not supported.

#### 2、Software dependency list


|  Dependency  | hyper-mpi  | Bisheng2.1 | gcc  | KML   |  
|  ---- | ----  | ---- | ---- | ----  |
| gcc  | Y(9.3.0) | Y(>4.8.5)| N | N |
| bisheng  | Y | N | N | N  |
| glibc-devel(libc-dev-bin) | N  | N | Y(>2.17) | N  |
| glibc  | N | Y(>2.17)| Y(>2.17) | N  |
| libatomic  | N | Y(>=1.2) | N | N  |

Note: The preceding table lists the dependencies required for installing the Hyper-MPI, math library, GCC, and Bisheng Compiler. Y indicates required dependency for the software and N indicates the dependency is not required.

## 3、Usage Guide

####  1、Obtain the kunpeng-hpc-1.0.0-aarc64-linux.tar.gz package. 
####  2、Run the following commands to install the software:
```
tar -xf kunpeng-hpc-1.0.0-aarch64-linux.tar.gz
cd kunpeng-hpc-1.0.0-aarch64-linux/script
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
> 4.3 Method 4: Make the environment variables take effect permanently.

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


## 4、Precautions

#### 1、The free drive space for the installation must be greater than 3 GB.
#### 2、The installation can be performed only in the AArch64 environment.

## 5、Software Uninstallation
#### 1、If environment variables are set after the installation, access /etc/profile or ~/.bashrc to check the directory where the binary or lib files of related software are stored.
```
vim /etc/profile（~/.bashrc）
export LD_LIBRARY_PATH=/usr/local/kml/lib:$LD_LIBRARY_PATH
...
export LD_LIBRARY_PATH=/usr/local/kml/lib/kspblas/multi:$LD_LIBRARY_PATH
export INCLUDE=/opt/gcc/gcc-xxxx-aarch64-linux/include:$INCLUDE
```
Note: After you access /etc/profile (~/.bashrc), you can view that the installation paths are /usr/local/kml and /opt/gcc/gcc-xxxx-aarch64-linux/ or Run the 'env / export' command to view the software installation path.


#### 2、Delete directories and environment variables based on the site requirements.

