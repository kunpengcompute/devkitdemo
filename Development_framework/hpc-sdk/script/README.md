# **鲲鹏HPC SDK一键式安装脚本使用**

简体中文 | [English](README_en.md)

## 1、介绍

#### 1、自动化安装脚本部署HPC相关软件。包括Hyper-mpi、kml、gcc、毕昇

#### 2、支持自定义路径安装


## 2、使用依赖

#### 1、服务器兼容性如下

| OS  | Hyper MPI  | 毕昇 | GCC for openEuler  | 数学库   |  
|----| ----  | ---- | ---- | ----  |
| centos 7  | Y (GCC9.3,毕昇2.1.0) | Y | Y | Y (GCC7.3及以上) |
| openEuler20.03 LTS  | Y (毕昇2.1.0) | Y | Y | Y (GCC7.3及以上) |
| openEuler20.03 sp1  |Y (GCC9.3,毕昇2.1.0) | Y | Y | Y (GCC7.3及以上) |
| openEuler20.03 sp2  | N | Y | Y | Y (GCC7.3及以上) |
| openEuler20.03 sp3  | N | Y | Y | Y (GCC7.3及以上) |
| 麒麟V10 SP1  | Y (GCC9.3,毕昇2.1.0) | Y | Y | N  |
| 麒麟V10 SP2  | Y (GCC9.3,毕昇2.1.0) | Y | Y | N  |
| unbuntu18.04  | N | Y | Y | Y (GCC7.3及以上)| 
| unbuntu20.04  | N | Y | Y | Y (GCC7.3及以上) |
| UOS 1020e | N | Y | Y | N  |  

说明：以上表中均为aarch64架构;小括号中为GCC、毕昇编译器的版本代表依赖最小版本，表中的Y代表系统支持安装，N代表不支持安装。

#### 2、软件依赖列表


|  依赖  | Hyper MPI  | 毕昇 | GCC for openEuler  | 数学库   |  
|  ---- | ----  | ---- | ---- | ----  |
| gcc  | Y(9.3.0) | Y(GCC4.8.5及以上)| N | N |
| bisheng  | Y | N | N | N  |
| glibc-devel(libc-dev-bin) | N  | N | Y(glibc-devel2.17及以上) | N  |
| glibc  | N | Y(glibc2.17及以上)| Y(glibc2.17及以上) | N  |
| libatomic  | N | Y(libatomic1.2.0及以上) | N | N  |  
| libgomp  | N | N | N | Y  |  

说明：以上表中内容为Hyper MPI、数学库、GCC for openEuler、毕昇编译器安装所需要的依赖，表中的Y代表系统依赖，N代表不依赖。redhat系安裝毕昇编译的rpm包时需确保系统已经安装libgomp。

## 3、使用教程

####  1、获取kunpeng-hpc-xxx-aarc64-linux.tar.gz包  
[https://mirrors.huaweicloud.com/kunpeng/archive/Kunpeng_SDK/HPC/kunpeng-hpc-1.0.1-aarch64-linux.tar.gz]
(https://mirrors.huaweicloud.com/kunpeng/archive/Kunpeng_SDK/HPC/kunpeng-hpc-1.0.1-aarch64-linux.tar.gz)
####  2、执行如下命令进行安装
```
tar -xf kunpeng-hpc-xxx-aarch64-linux.tar.gz
cd kunpeng-hpc-xxx-aarch64-linux/script
bash install.sh
```
#### 3、在执行安装过程中根据提示做相应的操作完成HPC安装  
#### 4、安装完成后根据提示添加环境变量：如下回显信息这里以gcc为例

> 4.1第一种方式执行下面命令添加环境变量到/etc/profile
```
  echo 'export PATH=/opt/gcc/gcc-xxx-aarch64-linux/bin:$PATH' >>/etc/profile
  echo 'export INCLUDE=/opt/gcc/gcc-xxx-aarch64-linux/include:$INCLUDE' >>/etc/profile
  echo 'export LD_LIBRARY_PATH=/opt/gcc/gcc-xxx-aarch64-linux/lib64:$LD_LIBRARY_PATH' >>/etc/profile
  source /etc/profile
```
> 4.2 第二种使用module加载module_file使环境变量生效
```
  module load /opt/modules/gcc/gcc_modulefiles
```
> 4.3 可以通过如下方式使变量永久生效  

> * 1、打开~/.bashrc 文件
```
  vim ~/.bashrc  
```
> * 2、把下面命令写入到 ~/.bashrc 
```
  module use /opt/modules/gcc/
  module load gcc_modulefiles
```
> * 3、保存文件  
> * 4、执行如下命令让配置生效 
```
  source ~/.bashrc
```
4.4 运行脚本配置环境变量
```
  cd kunpeng-hpc-xxx-aarch64-linux/script
  bash configure_environment_xxxx.sh
```

## 4、注意事项

#### 1、安装剩余磁盘空间应满足大于3GB
#### 2、仅支持在aarch64环境中安装

## 5、卸载软件
#### 以gcc和kml为例进行说明
#### 1、打开/etc/profile 或者 ~/.bashrc 检查相关软件二进制文件或者lib文件所在路径 。
```
vim /etc/profile（~/.bashrc）
export LD_LIBRARY_PATH=/usr/local/kml/lib:$LD_LIBRARY_PATH
...
export LD_LIBRARY_PATH=/usr/local/kml/lib/kspblas/multi:$LD_LIBRARY_PATH
export INCLUDE=/opt/gcc/gcc-xxxx-aarch64-linux/include:$INCLUDE
```

说明：执行上述命令打开/etc/profile（~/.bashrc）可以查看到具体的安装路径为‘/usr/local/kml’ 和 ‘/opt/gcc/gcc-xxxx-aarch64-linux/’
或者执行env / export 查看软件的安装路径。

#### 2、把/etc/profile 或者 ~/.bashrc中的gcc和kml环境变量删除。
#### 3、根据1步骤中获取的安装目录，在确认此目录中无其他要保留的文件后进行删除操作。路径中/opt为默认安装路径，若为其他自定义路径请自行替换。
#### 4、打开新的终端窗口，使修改的环境变量生效。
