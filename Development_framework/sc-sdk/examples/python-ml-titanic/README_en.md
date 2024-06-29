# python-ml-titanic demo

English | [简体中文](README.md)

## Introduction
python-ml-titanic is a reference implementation of Python machine learning developed using the high-level languages based on Kunpeng Confidential Computing.

[Introduction to Kunpeng Confidential Computing](https://www.hikunpeng.com/en/developer/boostkit/confidential-computing)

This demo is a secure computing high-level language application that uses the Python machine learning library to predict the survival data for the Titanic. It shows the application development and running process.

## Dependencies

1. Ensure that `kunpeng-sc-2.0.0` and `kunpeng-sc-devel-2.0.0` of the same version have been installed in the environment. Download the [Confidential Computing SDK](https://mirrors.huaweicloud.com/kunpeng/archive/Kunpeng_SDK/itrustee/).
2. Run the `lsmod | grep tzdriver` command to check whether the tzdriver is properly loaded.
3. Run the `ps -ef | grep teecd` command to check whether the daemon is started properly.

## Usage Guidance

1. Obtain the code.

    ```shell
    git clone https://github.com/kunpengcompute/devkitdemo.git
    ```

2. Go to the project root directory.

    ```shell
    cd ./devkitdemo/Development_framework/sc-sdk/examples/python-ml-titanic/
    ```

3. Pack the demo.
    Put the developer certificate (ta_cert.der) and private key (private_key.pem) to the `/usr/local/kunpeng-sc-devel/utils/pack-App/pack_tools/` directory.

    ```shell
    cp -rf titanic /usr/local/kunpeng-sc-devel/utils/pack-App/
    cd /usr/local/kunpeng-sc-devel/utils/pack-App/
    python3 pack.py titanic
    cd -
    cp /usr/local/kunpeng-sc-devel/utils/pack-App/titanic.sec ./
    ```

4. Install the dependencies. For details, see the section **Compiling Python Dependency Libraries Using the Confidential OS SDK**.

    ```shell
    # Signature
    cp -rf mllib /usr/local/kunpeng-sc-devel/utils/pack-App/
    cd /usr/local/kunpeng-sc-devel/utils/pack-App/
    python3 pack.py mllib
    cd -
    cp /usr/local/kunpeng-sc-devel/utils/pack-App/mllib.sec ./
    # Installation
    tee_teleport -m mllib.sec -t python
    ```

5. Install the demo.

    ```shell
    tee_teleport -c titanic.sec
    ```

6. Input the files required for running the demo.

    ```shell
    tee_teleport -n input -i sessionID.txt
    ```

7. Run the demo.

    ```shell
    tee_teleport -r titanic.py -i sessionID.txt
    ```

8. Obtain the log.

    ```shell
    tee_teleport -o output/tee.log -i sessionID.txt
    ```

9. Uninstall the demo.

    ```shell
    tee_teleport -e -i sessionID.txt
    ```

## Compiling Python Dependency Libraries Using the Confidential OS SDK

### Installing the Confidential OS SDK

```shell
mkdir -p /opt/ccos_build
cd /opt/ccos_build
wget -O BoostKit-ccos_sdk_1.0.0.zip https://kunpeng-repo.obs.cn-north-4.myhuaweicloud.com/Kunpeng%20BoostKit/Kunpeng%20BoostKit%2023.0.RC2/BoostKit-ccos_sdk_1.0.0.zip
unzip BoostKit-ccos_sdk_1.0.0.zip
tar -xf ccos_sdk.tgz
```

### Compiling and Installing Linux Python
```shell
wget -O Python-3.6.15.tgz https://www.python.org/ftp/python/3.6.15/Python-3.6.15.tgz
tar -zxf Python-3.6.15.tgz

cd Python-3.6.15
./configure --prefix=/opt/ccos_build/python_linux
make -j 4
make install
```

### Compiling and Installing Third-Party Libraries

Third-party library versions

|   Name         | Version |
| -------------- | ------- |
| openblas       | 0.3.9   |
| numpy          | 1.19.5  |
| scipy          | 1.5.4   |
| scikit-learn   | 0.24.2  |
| xgboost        | 1.5.1   |
| joblib         | 1.1.1   |

1. Download the code.

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

2. Install dependencies.

```shell
yum install gcc-gfortran
yum install openblas openblas-devel
```

3. Import environment variables.

```shell
export PYTHONPATH=/opt/ccos_build/python_linux
export PYTHONHOME=/opt/ccos_build/python_linux
export PATH=/opt/ccos_build/python_linux/bin:$PATH
```

4. Compile OpenBLAS on which NumPy depends.

```shell
cd /opt/ccos_build/build_mllib
bash -xe build_openblas.sh
```

5. Compile numpy.

```shell
cd /opt/ccos_build/build_mllib
tar -zxf numpy-1.19.5.tar.gz
cd numpy-1.19.5
cp site.cfg.example site.cfg
```

Modify the **site.cfg** file. Uncomment the following content and replace them to the actual directories of the OpenBLAS library:

```shell
sed -i '132s?# \[openblas\]?\[openblas\]?' site.cfg
sed -i '133s?# libraries = openblas?libraries = openblas?' site.cfg
sed -i '134s?# library_dirs = /opt/OpenBLAS/lib?library_dirs = /opt/ccos_build/build_mllib/build/lib?' site.cfg
sed -i '135s?# include_dirs = /opt/OpenBLAS/include?include_dirs = /opt/ccos_build/build_mllib/build/include?' site.cfg
```

After the replacement:
```
[openblas]
libraries = openblas
library_dirs = /opt/ccos_build/build_mllib/build/lib
include_dirs = /opt/ccos_build/build_mllib/build/include
```

Perform the compilation.
```shell
cd /opt/ccos_build/build_mllib
python3 -m pip install Cython==0.29.36
python3 -m pip install numpy==1.19.5
bash -xe build_numpy.sh /opt/ccos_build/ccos_sdk /opt/ccos_build/python_linux
```

6. Compile scipy.

```shell
cd /opt/ccos_build/build_mllib
tar -zxf scipy-1.5.4.tar.gz
cd scipy-1.5.4
cp site.cfg.example site.cfg
```

Modify the **site.cfg** file. Uncomment the following content and replace them to the actual directories of the OpenBLAS library:

```shell
sed -i '129s?# \[openblas\]?\[openblas\]?' site.cfg
sed -i '130s?# libraries = openblas?libraries = openblas?' site.cfg
sed -i '131s?# library_dirs = /opt/OpenBLAS/lib?library_dirs = /opt/ccos_build/build_mllib/build/lib?' site.cfg
sed -i '132s?# include_dirs = /opt/OpenBLAS/include?include_dirs = /opt/ccos_build/build_mllib/build/include?' site.cfg
```

After the replacement:
```
[openblas]
libraries = openblas
library_dirs = /opt/ccos_build/build_mllib/build/lib
include_dirs = /opt/ccos_build/build_mllib/build/include
```

Perform the compilation.

**Before the compilation, copy libgfortran.a to /opt/ccos_build/ccos_sdk/sysroots/ccos/usr/lib/aarch64-hongmeng-musl/7.3.0/.**

```shell
cd /opt/ccos_build/build_mllib
python3 -m pip install pybind11==2.10.4
bash -xe build_scipy.sh /opt/ccos_build/ccos_sdk /opt/ccos_build/python_linux
```

7. Compile xgboost.

```shell
cd /opt/ccos_build/build_mllib
bash -xe build_xgboost.sh /opt/ccos_build/ccos_sdk /opt/ccos_build/python_linux
```

8. Compile scikit-learn.

```shell
cd /opt/ccos_build/build_mllib
python3 -m pip install scipy==1.5.4
tar -zxf scikit-learn-0.24.2.tar.gz
bash -xe build_sklearn.sh /opt/ccos_build/ccos_sdk /opt/ccos_build/python_linux
```

9. Compile pandas.

```shell
cd /opt/ccos_build/build_mllib
python3 -m pip install pytz==2023.3
python3 -m pip install six==1.16.0
python3 -m pip install python-dateutil==2.8.2
python3 -m pip install joblib==1.1.1
tar -zxf pandas-1.1.5.tar.gz
```

To avoid the conflict between the identifiers in the SO file (dynamic library file) of pandas and those in the confidential OS, change `parser_init` to `pandas_parser_init`:

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

10. Compile joblib.

```shell
cd /opt/ccos_build/build_mllib
bash -xe build_joblib.sh /opt/ccos_build/ccos_sdk /opt/ccos_build/python_linux
```

11. Copy all third-party libraries to one directory.

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

# Copy pkg_resources
cp -rf /opt/ccos_build/python_linux/lib/python3.6/site-packages/pkg_resources ./mllib/
```

After the `mllib` directory is packaged and signed by using the signature tool, the directory can be deployed in the confidential OS.
