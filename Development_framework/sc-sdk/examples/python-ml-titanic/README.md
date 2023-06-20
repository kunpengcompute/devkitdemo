# python-ml-titanic demo

## 介绍
python-ml-titanic是使用鲲鹏机密计算高级语言特性开发的python机器学习参考实现

[鲲鹏机密计算特性介绍](https://www.hikunpeng.com/zh/developer/boostkit/confidential-computing)

此demo是一个使用Python机器学习库对泰坦尼克号生存数据进行预测的安全计算高级语言应用，展示了安全计算高级语言应用开发和运行的流程。

## 使用依赖

1. 确保环境上已安装机密计算SDK，需要同时安装相同版本的 `kunpeng-sc-2.0.0` 和 `kunpeng-sc-devel-2.0.0`，下载地址[机密计算SDK](https://mirrors.huaweicloud.com/kunpeng/archive/Kunpeng_SDK/itrustee/)
2. 确保tzdriver正常加载，执行 `lsmod | grep tzdriver` 查看
3. 确保守护进程正常启动，执行 `ps -ef | grep teecd` 查看

## 使用教程

1. 获取代码

    ```shell
    git clone https://github.com/kunpengcompute/devkitdemo.git
    ```

2. 进入到项目根路径

    ```shell
    cd ./devkitdemo/Development_framework/sc-sdk/examples/python-ml-titanic/
    ```

3. 打包
    先将开发者证书(ta_cert.der)和私钥(private_key.pem)放到`/usr/local/kunpeng-sc-devel/utils/pack-App/pack_tools/`目录下
    ```shell
    cp -rf titanic /usr/local/kunpeng-sc-devel/utils/pack-App/
    cd /usr/local/kunpeng-sc-devel/utils/pack-App/
    python3 pack.py titanic
    cd -
    cp /usr/local/kunpeng-sc-devel/utils/pack-App/titanic.sec ./
    ```

4. 安装依赖库, 可以参考 **使用鸿蒙SDK编译Python依赖库** 章节编译依赖库。

    ```shell
    # 签名
    cp -rf mllib /usr/local/kunpeng-sc-devel/utils/pack-App/
    cd /usr/local/kunpeng-sc-devel/utils/pack-App/
    python3 pack.py mllib
    cd -
    cp /usr/local/kunpeng-sc-devel/utils/pack-App/mllib.sec ./
    # 安装
    tee_teleport -m mllib.sec -t python
    ```

5. 安装

    ```shell
    tee_teleport -c titanic.sec
    ```

6. 运行

    ```shell
    tee_teleport -n input -r titanic.py -i sessionID.txt
    ```

7. 获取日志

    ```shell
    tee_teleport -o output/tee.log -i sessionID.txt
    ```

8. 卸载

    ```shell
    tee_teleport -e -i sessionID.txt
    ```

## 使用鸿蒙SDK编译Python依赖库

**编译脚本中涉及到的路径，请以自己环境上实际建立的路径为准并针对性的修改，比如SDK路径、Python安装路径、源码下载构建路径等等。文档和编译脚本中给出的是示例路径。**

三方库的版本确定流程：先在x86的python上用pip3安装三方库，用`pip3 freeze`查看版本，再找对应版本的三方库的源代码。 需要重新编译前，需要删除编译生成的中间文件，或者把源代码目录删除，重新解压，或者用`git clean -xdf`清理。

本文中的hongmeng指的是hongmeng运行环境，即交叉编译的产物要运行的环境。

Python编译

（1）在交叉编译前，首先在host机器上编译安装对应的Python版本，这里采用的Python版本为3.6.15

（2）host上编译Python
```shell
# 下载Python及三方库源代码的一般方法，以python3.6.15版本为例，后面其他的三方库下载方法类似，不再赘述
git clone https://github.com/python/cpython.git -b v3.6.15 --depth=1
cd cpython
git pull --unshallow
git submodule sync
git submodule update --init --recursive --jobs 0
# 在host上编译安装Python
#cd 到cpython目录，执行 ./configure --prefix=$HOME/pyinstall 其中prefix指定自定义的安装路径
make -j && make install
# 安装后设置环境变量
export PYTHONHOME=/home/xxx/pyinstall
export PATH=/home/xxx/pyinstall/bin:$PATH
#PYTHONPATH用于指定pip安装的包要安装在哪个位置
export PYTHONPATH=/home/xxx/pyinstall:/home/xxx/python_build/build/lib/python3.6/site-packages/

#使用python3 -V测试是否安装成功
```

编译安装Python后， 需要升级pip，使用命令`python3 -m pip install --upgrade pip`

(3) 交叉编译Python及Python自身依赖的三方库

安装鸿蒙sdk，作为交叉编译使用的工具链，这里假设安装路径为/home/xxx/hm-sdk

分别使用鸿蒙sdk编译Python依赖库：

| 依赖库    |  版本     | 下载地址 |
| ----     | ----      | --- |
| zlib     | 1.2.11    | https://github.com/madler/zlib.git |
| libffi   | 3.3-rc0   | https://github.com/libffi/libffi.git |
| libuuid  | 1.0.0     | https://sourceforge.net/projects/libuuid/files/libuuid-1.0.0.tar.gz/download |

编译完成后，复制每个依赖库安装目录的lib目录下的文件到鸿蒙sdk安装路径的/home/xxx/hm-sdk/sysroots/aarch64-euler-elf_all_in_one/usr/lib下，复制每个依赖库安装目录的include目录下的文件到鸿蒙sdk安装路径的/home/xxx/hm-sdk/sysroots/aarch64-euler-elf_all_in_one/usr/include下。

使用第一步下载的Python源代码，解压后进入源码目录下编译Python

```shell
source /home/xxx/hm-sdk/environment-setup-aarch64-euler-elf
./configure --prefix=/home/xxx/pyintall CFLAGS="-D_GNE_SOURCE -fwrapv -DFFI_MMAP_EXEC_WRIT -I/home/xxx/hm-sdk/sysroots/aarch64-euler-elf_all_in_one/usr/include" LDFLAGS="-L/home/xxx/hm-sdk/sysroots/aarch64-euler-elf_all_in_one/usr/lib/ -Wl,-rpath /home/xxx/hm-sdk/sysroots/aarch64-euler-elf_all_in_one/usr/lib/" --host=aarch64-linux-gnu --build=x86_64-linux-gnu --enable-shared -disable-ipv6 ac_cv_file__dev_ptmx=yes ac_cv_file__dev_ptc=no
make -j 4
make install
cp -ar /home/xxx/pyinstall/lib/* /home/xxx/hm-sdk/sysroots/aarch64-euler-elf_all_in_one/usr/lib/
cp -ar /home/xxx/pyinstall/include/* /home/xxx/hm-sdk/sysroots/aarch64-euler-elf_all_in_one/usr/include/
./build_3.6.sh /home/xxx/hm-sdk /home/xxx/pyinstal
```

交叉编译好的Python安装在python_build/output_python目录下，拷贝出来备用


### 编译numpy 1.19.5

```shell
#首先在host机器上安装numpy和Cython
pip3 install numpy==1.19.5
pip3 install Cython
#进入到hm-sdk目录，刷新环境变量
source environment-setup-aarch64-euler-elf
#指定Fortran编译器
cd hm-sdk/sysroots/x86_64-eulersdk-linux/usr/bin
ln -s aarch64-hongmeng-musl-gfortran gfortran
# 确保gfortran在PATH路径里，可以通过在shell下执行gfortran确认
#交叉编译numpy前需要先交叉编译openblas库，下载版本tag为0.3.9的源代码
export PATH=/home/xxx/hm-sdk/sysroots/x86_64-eulersdk-linux/usr/bin:$PATH
export CFLAGS="-fstack-protector-strong -O2 -pipe -D__linux__ -D_GNU_SOURCE -fPIC -fwrapv -DOS_LINUX"
cd 到openblas源码根目录
make TARGET=ARMV8 HOSTCC=gcc BINARY=64 CC=aarch64-euler-elf-gcc FC=aarch64-euler-elf-gfortran
OSNAME=LINUX
# .a转换为.so
cp /home/xxx/hm-sdk/sysroots/x86_64-eulersdk-linux/usr/aarch64-hongmeng-musl/lib64/libgfortran.a .
aarch64-euler-elf-gcc -shared -o libgfortran.so -Wl,--whole-archive libgfortran.a -Wl,--no-whole-archive
aarch64-euler-elf-gcc -shared -o libopenblas_armv8p-r0.3.9.so -Wl,--whole-archive libopenblas_armv8pr0.3.9.a libgfortran.a -Wl,--no-whole-archive
#安装交叉编译好的openblas到指定目录
make PREFIX=/home/xxx/myblas TARGET=ARMV8 install
#从GitHub下载numpy源代码，指定tag为v1.19.5，方法同Python源码下载
cd到numpy很目录
cp site.cfg.example site.cfg
#编辑site.cfg文件，放开以下注释并修改为实际的路径
129 [openblas]
130 libraries = openblas
131 library_dirs = /home/xxx/myblas/lib

132 include_dirs = /home/xxx/myblas/include
#在Python_build下执行
source /home/xxx/hm-sdk/environment-setup-aarch64-euler-elf
python3 setup.py install --prefix=/home/xxx/build/
#交叉编译好的numpy会安装在python_build/build目录下，拷贝整个文件夹出来备用，这个文件夹就是可以在鸿蒙上运行的numpy库
```

### 编译scipy 1.5.4

```shell
#首先在host上安装pybind11
pip3 install pybind11
#下载scipy的源码，指定tag为1.5.4
cd 到scipy的根目录
# 修改site.cfg，修改方式同numpy
#在Python_build下执行
source /home/xxx/hm-sdk/environment-setup-aarch64-euler-elf
python3 setup.py install --prefix=/home/xxx/build/
#编译过程中如果遇到cannot find -lnpymath的错误，需要把交叉编译好的numpy/core/lib/libnpymath.a拷贝到pyinstall的numpy目录下
cd /home/xxx/pyinstall/lib/python3.6/site-packages/numpy/core/lib
mv libnpymath.a libnpymath.a.bak
cp xxx/libnpymath.a /home/xxx/pyinstall/lib/python3.6/site-packages/numpy/core/lib
#其中xxx/libnpymath.a是上一步交叉编译出来的静态库
```

### 编译xgboost 1.6.2

这里采用关闭openmp的编译方法，如果使用openmp需测试是否能产生正确的计算结果。

```shell
# git下载源代码，版本用1.6.2
# 修改编译脚本 CMakeLists.txt
project(xgboost LANGUAGES CXX C VERSION 1.6.2)

### hongmeng build
set(CMAKE_C_COMPILER "/home/xxx/hm-sdk/sysroots/x86_64-eulersdk-linux/usr/bin/aarch64-euler-elf-gcc")
set(CMAKE_CXX_COMPILER "/home/xxx/hm-sdk/sysroots/x86_64-eulersdk-linux/usr/bin/aarch64-euler-elf-g
++")
set(CMAKE_SYSROOT "/home/xxx/hm-sdk/sysroots/aarch64-euler-elf")
add_compile_options("-march=armv8-a")
add_compile_options("-mlittle-endian")
set(CMAKE_LINKER "/home/xxx/hm-sdk/sysroots/x86_64-eulersdk-linux/usr/bin/aarch64-euler-elf-ld")
add_definitions(-D__GLIBC__)
# 修改
option(USE_OPENMP "Build with OpenMP support." OFF)

# 修改python-package/setup.py
'use-openmp': (None, 'Build with OpenMP support.', 0),

self.use_openmp = 0

if shutil.which('ninja'):
 build_tool = 'make'

# 修改include/xgboost/base.h57到61行修改如下：（删除原来的include和XGBOOST_PARALLEL_SORT等定义，使用std::sort）
#if defined(__GNUC__) && ((__GNUC__ == 4 && __GNUC_MINOR__ >= 8) || __GNUC__ > 4) && \
 !defined(__CUDACC__) && !defined(__sun) && !defined(sun)
#define XGBOOST_PARALLEL_SORT(X, Y, Z) std::sort((X), (Y), (Z))
#define XGBOOST_PARALLEL_STABLE_SORT(X, Y, Z) std::stable_sort((X), (Y), (Z))
#elif defined(_MSC_VER) && (!__INTEL_COMPILER)
# libgomp目前还不支持

#在Python_build下执行
source /home/xxx/hm-sdk/environment-setup-aarch64-euler-elf
python3 setup.py install --prefix=/home/xxx/build/
```

### 编译sklearn 0.24.2

```shell
#git下载代码 版本0.24.2
#在环境变量中加入
export PYTHON_CROSSENV=1
# 因为sklearn的Makefile里直接用了python，在~/.bin/里加入python软连接指向python3.6，并加入PATH
#编译前现在host上安装scipy和sklearn
pip3 install scipy==1.5.4
pip3 install scikit-learn==0.24.2
#在Python_build下执行
source /home/xxx/hm-sdk/environment-setup-aarch64-euler-elf
python3 setup.py install --prefix=/home/xxx/build/
# 如果出现问题
ImportError: cannot import name '_lsap_module'
# 将lib/python3.6/site-packages/scipy/optimize/_lsap_module.cpython-36m-x86_64-linux-gnu.so重命名为
_lsap_module.so
```

### 编译pandas 1.1.5

```shell
#git下载代码pandas-1.1.5
# 先在host机器上安装一些依赖库，也可以在编译过程中自动安装
pip3 install pytz
pip3 install python-dateutil
pip3 install six==1.16.0
#在Python_build下执行
source /home/xxx/hm-sdk/environment-setup-aarch64-euler-elf
python3 setup.py install --prefix=/home/xxx/build/
```

### 编译joblib 1.1.1

```shell
# git下载joblib代码，版本为1.1.1
#在Python_build下执行
source /home/xxx/hm-sdk/environment-setup-aarch64-euler-elf
python3 setup.py install --prefix=/home/xxx/build/
```

### 其他三方库编译

可以直接把x86三方库复制到hongmeng使用的有six，threadpoolctl，date-util。这些库都是纯Python文件，所以不需要交叉编译。

### 运行指导

准备交叉编译好的Python解释器目录，为了让Python能在鸿蒙上运行，需要从SDK里拷贝一些库文件到Python解释器的lib目录。

```shell
# 从编译好的openblas目录拷贝文件到Python解释器目录(以下命令为示例，需要根据实际的路径修改使用,hm-sdk指SDK的路径，output_python指代编译好的Python解释器路径)：
cd openblas/lib
cp libopenblas_armv8p-r0.3.9.so output_python/lib/libopenblas.so
cp libgfortran.so output_python/lib/
# 如果运行时提示找不到某个库，首先考虑去SDK中搜索这个库的名字，并把相应的库文件拷贝到Python解释器lib目录下，注意不要拷贝软链接文件；其次考虑当前Python解释器目录下对应的库的命名是否正确。比如/lib/python3.6/site-packages/scipy/optimize/lsap_module.cpython-36m-x86_64-linux-gnu.so，需要改名为lsap_module.cpython-36m.so，这样在鸿蒙运行环境中才能正确识别
```
