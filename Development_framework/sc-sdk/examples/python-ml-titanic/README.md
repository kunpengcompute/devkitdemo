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

4. 安装依赖库, 可以参考 **使用机密OS SDK编译Python依赖库** 章节编译依赖库。

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

6. 输入文件

    ```shell
    tee_teleport -n input -i sessionID.txt
    ```

7. 运行

    ```shell
    tee_teleport -r titanic.py -i sessionID.txt
    ```

8. 获取日志

    ```shell
    tee_teleport -o output/tee.log -i sessionID.txt
    ```

9. 卸载

    ```shell
    tee_teleport -e -i sessionID.txt
    ```

## 使用机密OS SDK编译Python依赖库

### 安装机密OS SDK

```shell
mkdir -p /opt/ccos_build
cd /opt/ccos_build
wget -O BoostKit-ccos_sdk_1.0.0.zip https://kunpeng-repo.obs.cn-north-4.myhuaweicloud.com/Kunpeng%20BoostKit/Kunpeng%20BoostKit%2023.0.RC2/BoostKit-ccos_sdk_1.0.0.zip
unzip BoostKit-ccos_sdk_1.0.0.zip
tar -xf ccos_sdk.tgz
```

### 编译安装Linux Python
```shell
wget -O Python-3.6.15.tgz https://www.python.org/ftp/python/3.6.15/Python-3.6.15.tgz
tar -zxf Python-3.6.15.tgz

cd Python-3.6.15
./configure --prefix=/opt/ccos_build/python_linux
make -j 4
make install
```

### 编译安装第三方库

依赖的第三方库版本

|   name         | version |
| -------------- | ------- |
| openblas       | 0.3.9   |
| numpy          | 1.19.5  |
| scipy          | 1.5.4   |
| scikit-learn   | 0.24.2  |
| xgboost        | 1.5.1   |
| joblib         | 1.1.1   |

1. 下载代码

```shell
cp -rf build_mllib /opt/ccos_build/
cd /opt/ccos_build/build_mllib
wget https://github.com/scikit-learn/scikit-learn/archive/refs/tags/0.24.2.tar.gz -O scikit-learn-0.24.2.tar.gz
wget https://github.com/scipy/scipy/archive/refs/tags/v1.5.4.tar.gz -O scipy-1.5.4.tar.gz
wget https://github.com/pandas-dev/pandas/archive/refs/tags/v1.1.5.tar.gz -O pandas-1.1.5.tar.gz
wget https://github.com/numpy/numpy/archive/refs/tags/v1.19.5.tar.gz -O numpy-1.19.5.tar.gz
wget https://github.com/joblib/joblib/archive/refs/tags/1.1.1.tar.gz -O joblib-1.1.1.tar.gz
wget https://github.com/xianyi/OpenBLAS/archive/refs/tags/v0.3.9.tar.gz -O OpenBLAS-0.3.9.tar.gz
git clone https://github.com/dmlc/xgboost.git -b v1.5.1
cd xgboost
git submodule sync
git submodule update --init --recursive --jobs 0
```

2. 安装依赖软件

```shell
yum install gcc-gfortran
yum install openblas openblas-devel
```

3. 导入环境变量

```shell
export PYTHONPATH=/opt/ccos_build/python_linux
export PYTHONHOME=/opt/ccos_build/python_linux
export PATH=/opt/ccos_build/python_linux/bin:$PATH
```

4. 编译numpy依赖软件openblas

```shell
cd /opt/ccos_build/build_mllib
bash -xe build_openblas.sh
```

5. 编译numpy

```shell
cd /opt/ccos_build/build_mllib
tar -zxf numpy-1.19.5.tar.gz
cd numpy-1.19.5
cp site.cfg.example site.cfg
```

编辑site.cfg文件，放开以下注释并修改为openblas库的实际路径

```shell
sed -i '132s?# \[openblas\]?\[openblas\]?' site.cfg
sed -i '133s?# libraries = openblas?libraries = openblas?' site.cfg
sed -i '134s?# library_dirs = /opt/OpenBLAS/lib?library_dirs = /opt/ccos_build/build_mllib/build/lib?' site.cfg
sed -i '135s?# include_dirs = /opt/OpenBLAS/include?include_dirs = /opt/ccos_build/build_mllib/build/include?' site.cfg
```

替换完成后如下：
```
[openblas]
libraries = openblas
library_dirs = /opt/ccos_build/build_mllib/build/lib
include_dirs = /opt/ccos_build/build_mllib/build/include
```

执行编译
```shell
cd /opt/ccos_build/build_mllib
python3 -m pip install Cython==0.29.36
python3 -m pip install numpy==1.19.5
bash -xe build_numpy.sh /opt/ccos_build/ccos_sdk /opt/ccos_build/python_linux
```

6. 编译scipy

```shell
cd /opt/ccos_build/build_mllib
tar -zxf scipy-1.5.4.tar.gz
cd scipy-1.5.4
cp site.cfg.example site.cfg
```

编辑site.cfg文件，放开以下注释并修改为openblas库的实际路径

```shell
sed -i '129s?# \[openblas\]?\[openblas\]?' site.cfg
sed -i '130s?# libraries = openblas?libraries = openblas?' site.cfg
sed -i '131s?# library_dirs = /opt/OpenBLAS/lib?library_dirs = /opt/ccos_build/build_mllib/build/lib?' site.cfg
sed -i '132s?# include_dirs = /opt/OpenBLAS/include?include_dirs = /opt/ccos_build/build_mllib/build/include?' site.cfg
```

替换完成后如下：
```
[openblas]
libraries = openblas
library_dirs = /opt/ccos_build/build_mllib/build/lib
include_dirs = /opt/ccos_build/build_mllib/build/include
```

执行编译

**需要先将libgfortran.a复制到/opt/ccos_build/ccos_sdk/sysroots/ccos/usr/lib/aarch64-hongmeng-musl/7.3.0/目录下**

```shell
cd /opt/ccos_build/build_mllib
python3 -m pip install pybind11==2.10.4
bash -xe build_scipy.sh /opt/ccos_build/ccos_sdk /opt/ccos_build/python_linux
```

7. 编译xgboost

```shell
cd /opt/ccos_build/build_mllib
bash -xe build_xgboost.sh /opt/ccos_build/ccos_sdk /opt/ccos_build/python_linux
```

8. 编译scikit-learn

```shell
cd /opt/ccos_build/build_mllib
python3 -m pip install scipy==1.5.4
tar -zxf scikit-learn-0.24.2.tar.gz
bash -xe build_sklearn.sh /opt/ccos_build/ccos_sdk /opt/ccos_build/python_linux
```

9. 编译pandas

```shell
cd /opt/ccos_build/build_mllib
python3 -m pip install pytz==2023.3
python3 -m pip install six==1.16.0
python3 -m pip install python-dateutil==2.8.2
python3 -m pip install joblib==1.1.1
tar -zxf pandas-1.1.5.tar.gz
```

为了避免pandas的so中的符号与机密OS中的符号冲突，需要将parser_init修改为pandas_parser_init

```
pandas/_libs/src/parser/tokenizer.c:146:int parser_init(parser_t *self) {
pandas/_libs/src/parser/tokenizer.h:184:int parser_init(parser_t *self);
pandas/_libs/parsers.pyx:224:    int parser_init(parser_t *self) nogil
pandas/_libs/parsers.pyx:385:        parser_init(self.parser)
```

```
pandas/_libs/src/parser/tokenizer.c:146:int pandas_parser_init(parser_t *self) {
pandas/_libs/src/parser/tokenizer.h:184:int pandas_parser_init(parser_t *self);
pandas/_libs/parsers.pyx:224:    int pandas_parser_init(parser_t *self) nogil
pandas/_libs/parsers.pyx:385:        pandas_parser_init(self.parser)
```

```shell
bash -xe build_pandas.sh /opt/ccos_build/ccos_sdk /opt/ccos_build/python_linux
```

10. 编译joblib

```shell
cd /opt/ccos_build/build_mllib
bash -xe build_joblib.sh /opt/ccos_build/ccos_sdk /opt/ccos_build/python_linux
```

11. 复制所有第三方库到一个目录

```shell
cd /opt/ccos_build/build_mllib
mkdir mllib
cp -rf build/lib/python3.6/site-packages/joblib-1.1.1-py3.6.egg/joblib/ ./mllib/
cp -rf build/lib/python3.6/site-packages/numpy-1.19.5-py3.6-linux-aarch64.egg/numpy/ ./mllib/
cp -rf build/lib/python3.6/site-packages/pandas-1.1.5-py3.6-linux-aarch64.egg/pandas/ ./mllib/
cp -rf build/lib/python3.6/site-packages/scikit_learn-0.24.2-py3.6-linux-aarch64.egg/sklearn/ ./mllib/
cp -rf build/lib/python3.6/site-packages/scipy-1.5.4-py3.6-linux-aarch64.egg/scipy/ ./mllib/
cp -rf build/lib/python3.6/site-packages/threadpoolctl-3.2.0-py3.6.egg/threadpoolctl.py ./mllib/
cp -rf build/lib/python3.6/site-packages/xgboost ./mllib/
cp -rf build/lib/python3.6/site-packages/site.py ./mllib/

cp -rf /opt/ccos_build/python_linux/lib/python3.6/site-packages/dateutil/ ./mllib/
cp -rf /opt/ccos_build/python_linux/lib/python3.6/site-packages/pytz/ ./mllib/
cp -rf /opt/ccos_build/python_linux/lib/python3.6/site-packages/six.py ./mllib/

# 复制pkg_resources
cp -rf /opt/ccos_build/python_linux/lib/python3.6/site-packages/pkg_resources ./mllib/
```

使用签名工具将mllib目录进行打包签名后，就可以部署到机密OS中使用了。
