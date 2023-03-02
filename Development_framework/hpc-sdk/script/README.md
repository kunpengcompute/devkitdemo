# **鲲鹏HPC SDK一键式安装脚本使用**

简体中文 | [English](README_en.md)

## 1、介绍

#### 1、自动化安装脚本部署HPC相关软件。包括Hyper MPI、KML、GCC for openEuler、毕昇

#### 2、支持自定义路径安装


## 2、使用依赖

#### 1、服务器兼容性如下

|OS  |Hyper MPI  |毕昇 |GCC for openEuler  |KML   |  
|----| ----  | ---- | ---- | ----  |
| CentOS 7.6 | Y (GCC9.3.0,毕昇2.1.0) | Y | Y | Y (GCC7.3.0及以上) |
| openEuler 20.03 LTS  | Y (毕昇2.1.0) | Y | Y | Y (GCC7.3.0及以上) |
| openEuler 20.03 SP1  | Y (GCC9.3.0,毕昇2.1.0) | Y | Y | Y (GCC7.3.0及以上) |
| openEuler 20.03 SP2  | N | Y | Y | Y (GCC7.3.0及以上) |
| openEuler 20.03 SP3  | N | Y | Y | Y (GCC7.3.0及以上) |
| 麒麟V10 SP1  | Y (GCC9.3.0,毕昇2.1.0) | Y | Y | N  |
| 麒麟V10 SP2  | Y (GCC9.3.0,毕昇2.1.0) | Y | Y | N  |
| Unbuntu18.04  | N | Y | Y | Y (GCC7.3.0及以上)| 
| Unbuntu20.04  | N | Y | Y | Y (GCC7.3.0及以上) |
| UOS 1020e | N | Y | Y | N  |  

说明：以上表中OS均为aarch64架构，表中的"Y"代表当前OS支持安装，"N"代表当前OS不支持安装。

#### 2、软件依赖列表


| 依赖  |Hyper MPI  |毕昇 |GCC for openEuler  |KML  |  
|  ---- | ----  | ---- | ---- | ----  |
| GCC  | Y(9.3.0) | Y(GCC4.8.5及以上)| N | N |
| BiSheng  | Y | N | N | N  |
| glibc-devel(libc-dev-bin) | N  | N | Y(glibc-devel2.17及以上) | N  |
| glibc  | N | Y(glibc2.17及以上)| Y(glibc2.17及以上) | N  |
| libatomic  | N | Y(libatomic1.2.0及以上) | N | N  |  
| libgomp  | N | N | N | Y  |  
| libgomp1  | N | N | N | Y |  

说明：以上表中内容为Hyper MPI、KML、GCC for openEuler、毕昇编译器安装所需要的依赖，表中的"Y"代表系统依赖，"N"代表不依赖。  

安装RedHat系的rpm包时需确保系统已经安装libgomp。安装Debian系的deb包时需确保系统已经安装libgomp1。

## 3、使用教程

####  1、获取kunpeng-hpc-xxx-aarc64-linux.tar.gz包  
[https://mirrors.huaweicloud.com/kunpeng/archive/Kunpeng_SDK/HPC/kunpeng-hpc-1.0.1-aarch64-linux.tar.gz]
(https://mirrors.huaweicloud.com/kunpeng/archive/Kunpeng_SDK/HPC/kunpeng-hpc-1.0.1-aarch64-linux.tar.gz)
####  2、执行如下命令进行安装
```
tar -zxf kunpeng-hpc-xxx-aarch64-linux.tar.gz
cd kunpeng-hpc-xxx-aarch64-linux/script
bash install.sh
```
#### 3、在执行安装过程中根据提示进行操作，完成HPC SDK安装
#### 4、安装完成后根据提示添加环境变量：如下回显信息这里以GCC for openEuler为例

>方式1：执行下面命令添加环境变量到/etc/profile
```
  echo 'export PATH=/opt/gcc/gcc-xxx-aarch64-linux/bin:$PATH' >>/etc/profile
  echo 'export INCLUDE=/opt/gcc/gcc-xxx-aarch64-linux/include:$INCLUDE' >>/etc/profile
  echo 'export LD_LIBRARY_PATH=/opt/gcc/gcc-xxx-aarch64-linux/lib64:$LD_LIBRARY_PATH' >>/etc/profile
  source /etc/profile
```
>方式2：使用module加载module_file使环境变量生效
```
  module load /opt/modules/gcc/gcc_modulefiles
```
>方式3：可以通过如下方式使变量永久生效  

>>1、打开~/.bashrc 文件
```
  vim ~/.bashrc  
```
>>2、把下面命令写入到 ~/.bashrc 
```
  module use /opt/modules/gcc/
  module load gcc_modulefiles
```
>> 3、保存文件  
>> 4、执行如下命令让配置生效 
```
  source ~/.bashrc
```
>方式4：运行脚本配置环境变量
```
  cd kunpeng-hpc-xxx-aarch64-linux/script
  bash configure_environment_xxxx.sh
```

## 4、注意事项

#### 1、仅支持在aarch64环境中安装。
#### 2、安装时剩余磁盘空间应满足大于3GB，不仅针对只安装其中一个软件应满足大于3G，还包含Hyper MPI、KML、GCC for openEuler以及毕昇编译器时也应满足磁盘空间大于3G。

## 5、卸载HPC SDK
#### 1、SDK安装成功如果有设置环境变量，打开/etc/profile 或者 ~/.bashrc 检查相关软件二进制文件或者lib文件所在路径。
```
vim /etc/profile（~/.bashrc）
export LD_LIBRARY_PATH=/usr/local/kml/lib:$LD_LIBRARY_PATH
...
export LD_LIBRARY_PATH=/usr/local/kml/lib/kspblas/multi:$LD_LIBRARY_PATH
export INCLUDE=/opt/gcc/gcc-xxxx-aarch64-linux/include:$INCLUDE
```

说明：执行上述命令打开/etc/profile（~/.bashrc）可以查看到KML的安装路径为‘/usr/local/kml’，GCC for openEuler的安装路径为‘/opt/gcc/gcc-xxxx-aarch64-linux’。  

也可执行```env|export``` 查看软件对应的安装路径。

#### 2、把/etc/profile 或者 ~/.bashrc中的GCC for openEuler和KML环境变量删除。
#### 3、根据1步骤中获取的安装目录，在确认‘/usr/local/kml’和‘/opt/gcc/gcc-xxxx-aarch64-linux’目录中无需要保留的文件后进行删除操作。路径中/opt为默认安装路径，若为其他自定义路径请自行替换。
#### 4、打开新的终端窗口，使修改的环境变量生效。
