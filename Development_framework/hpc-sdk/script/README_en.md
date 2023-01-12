# **Kunpeng HPC SDK One-Click Installation Script**

English | [简体中文](README.md)

## 1、Introduction

#### 1、Use the automatic installation script to automatically deploy HPC software, including Hyper MPI, KML, GCC, and BISHENG.

#### 2、Support installation in a user-defined path.


## 2、Dependency

#### 1、The server compatibility is as follows:

|  OS  | Hyper MPI  | BISHENG | GCC for openEuler  | KML   |  
|  ---- | ----  | ---- | ---- | ----  |
| CentOS 7.6  | Y (GCC 9.3.0,BISHENG 2.1.0) | Y | Y | Y (GCC 7.3.0 or later) |
| openEuler20.03 LTS  | Y (BISHNEG 2.1.0) | Y | Y | Y (GCC 7.3.0 or later) |
| openEuler20.03 SP1  | Y (GCC 9.3.0,BISHENG 2.1.0) | Y | Y | Y (GCC 7.3.0 or later) |
| openEuler20.03 SP2  | N | Y | Y | Y (GCC 7.3.0 or later) |
| openEuler20.03 SP3  | N | Y | Y | Y (GCC 7.3.0 or later) |
| kylin V10 SP1  | Y (GCC 9.3.0,BISHENG 2.1.0) | Y | Y | N  |
| kylin V10 SP2  | Y (GCC 9.3.0,BISHENG 2.1.0) | Y | Y | N  |
| unbuntu18.04  | N | Y | Y | Y (GCC 7.3.0 or later)| 
| unbuntu20.04  | N | Y | Y | Y (GCC 7.3.0 or later) |
| UOS 1020e | N | Y | Y | N  | 

Note: The OSs in the preceding table use the AArch64 architecture. Y indicates that the OS supports the installation, and N indicates that the OS does not support the installtion.

#### 2、Software dependency list


|  Dependency  | Hyper MPI  | BISHENG | GCC for openEuler  | KML   |  
|  ---- | ----  | ---- | ---- | ----  |
| GCC  | Y(GCC 9.3.0) | Y(GCC 4.8.5 or later)| N | N |
| BiSheng  | Y | N | N | N  |
| glibc-devel(libc-dev-bin) | N  | N | Y(glibc-devel 2.17 or later) | N  |
| glibc  | N | Y(glibc 2.17 or later)| Y(glibc 2.17 or later) | N  |
| libatomic  | N | Y(libatomic 1.2 or later) | N | N  |
| libgomp  | N | N | N | Y  | 
| libgomp1  | N | N | N | Y  | 

Note: The preceding table lists the dependencies required for installing the Hyper MPI, kML, GCC for openEuler, and Bisheng Compiler. Y indicates required dependency for the software and N indicates the dependency is not required. Before installing the RPM package compiled by BiSheng in the Red Hat system, Ensure that libgomp has been installed before installing an RPM package on a Red Hat OS and that libgomp1 has been installed before installing a DEB package on a Debian OS.

## 3、Usage Guide

####  1、Obtain the kunpeng-hpc-1.0.0-aarc64-linux.tar.gz package. 
[https://mirrors.huaweicloud.com/kunpeng/archive/Kunpeng_SDK/HPC/kunpeng-hpc-1.0.1-aarch64-linux.tar.gz]
(https://mirrors.huaweicloud.com/kunpeng/archive/Kunpeng_SDK/HPC/kunpeng-hpc-1.0.1-aarch64-linux.tar.gz)
####  2、Run the following commands to install the software:
```
tar -xf kunpeng-hpc-xxx-aarch64-linux.tar.gz
cd kunpeng-hpc-xxx-aarch64-linux/script
bash install.sh
```
#### 3、Perform operations as prompted to install the HPC software. 
#### 4、After the installation is complete, add environment variables as prompted. The following command output uses GCC for openEuler as an example:

> Method 1: Add environment variables to /etc/profile.
```
  echo 'export PATH=/opt/gcc/gcc-xxx-aarch64-linux/bin:$PATH' >>/etc/profile
  echo 'export INCLUDE=/opt/gcc/gcc-xxx-aarch64-linux/include:$INCLUDE' >>/etc/profile
  echo 'export LD_LIBRARY_PATH=/opt/gcc/gcc-xxx-aarch64-linux/lib64:$LD_LIBRARY_PATH' >>/etc/profile
  source /etc/profile
```
> Method 2: Using the module command to load module_file for the environment variables to take effect.
```
  module load /opt/modules/gcc/gcc_modulefiles
```
> Method 3: Make the environment variables take effect permanently.

>> 1、Open the ~/.bashrc.
```
  vim ~/.bashrc  
```
>> 2、Add the following content to the ~/.bashrc file:
```
  module use /opt/modules/gcc
  module load gcc_modulefiles
```
>> 3、Save the file.
>> 4、Make the configuration take effect:
```
  source ~/.bashrc
```
Method 4: Running a script to configure environment variables.
```
  cd kunpeng-hpc-xxx-aarch64-linux/script
  bash configure_environment_xxxxx.sh
```
## 4、Precautions

#### 1、The installation can be performed only in the aarch64 environment.
#### 2、Reserve at least 3GB drive sapace after installing Hyper MPI,KML,GCC for openEuler ,and Bisheng compiler or any of them.

## 5、Uninstalling the HPC SDK 

#### 1、After the SDk is installed successfully, if environment variables are set, open /etc/profile or ~/.bashrc to check the path of the binary file or lib file of related software.
```
vim /etc/profile（~/.bashrc）
export LD_LIBRARY_PATH=/usr/local/kml/lib:$LD_LIBRARY_PATH
...
export LD_LIBRARY_PATH=/usr/local/kml/lib/kspblas/multi:$LD_LIBRARY_PATH
export INCLUDE=/opt/gcc/gcc-xxxx-aarch64-linux/include:$INCLUDE
```

Note: In the command output of vim /etc/profile (~/.bashrc), the installation path of the KML is /usr/local/kml and path of GCC for openEuler is /opt/gcc/gcc-xxxx-aarch64-linux.  
You can also run env or export to query the installation paths.

#### 2、Delete environment variables.
#### 3、Make sure that no files need to be retained in the installation directories (/usr/local/kml and /opt/gcc/gcc-xxxx-aarch64-linux in this example) and then delete these directories.  
'/opt' is the default installation path. which needs to be changed if you have customized the installation path.  
4、Open a new terminal window for the modified environment variables to take effect.
