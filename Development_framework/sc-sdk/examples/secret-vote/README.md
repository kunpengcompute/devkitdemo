# secret-vote demo

简体中文 | [English](README_en.md)

## 介绍
secret-vote demo是使用鲲鹏机密计算特性开发的匿名投票系统的参考实现

[鲲鹏机密计算特性介绍](https://www.hikunpeng.com/zh/developer/boostkit/confidential-computing)

项目主要分为4个模块：
- CA Client Application，客户端应用程序，编译生成libsecret-vote-ca.so供Web服务调用
- TA Trusted Application，可信应用程序，运行在安全操作系统，使用GP接口（GlobalPlatform，机密计算标准规范）完成加解密和数据保存
- Service Web服务，使用Python的Django框架开发的Web服务，提供REST API，需要调用CA编译生成的libsecret-vote-ca.so实现公私钥对的申请和投票数据的加解密
- UI 前端，使用vue和element-ui开发的投票UI，支持用户登录、退出、投票、投票结果展示

## 主要目录说明
- CA       CA源码
- TA       TA源码
- ui       前端源码，用户可以使用源码自行打包构建
- service  后端源码
- static   前端静态文件，构建好的静态文件，不构建也可直接使用
- lib      编译CA时创建，CA编译输出的so存放的目录，Web服务从此目录中加载CA生成的so
- pubkey   用户登录和投票过程中生成，存储根公钥和用户公钥
- build    构建脚本

## 投票业务流程

1. 初始化
    - 创建10个用户
    - 调用CA的接口申请根公私钥对
    - CA调用TA的GP接口生成RSA根公私钥对
    - TA使用GP的安全存储API保存根公私钥对，并将根公钥返回给CA
    - CA使用openssl接口将根公钥存储为pem格式的文件
2. 用户登录
    - 判断用户是否申请过用户公钥，如果未申请过，申请公钥
    - 调用CA的接口申请用户公钥和签名
    - CA调用TA的GP接口生成RSA用户公钥对
    - TA使用GP的安全存储API保存用户公私钥对
    - TA读取根公私钥对对用户公钥进行签名，将用户公钥和签名数据返回给CA
    - CA使用openssl接口将用户公钥存储为pem格式的文件，并将签名数据返回给Web端
    - Web端将签名数据保存到数据库中
3. 投票
    - Web端读取用户公钥和签名，调用CA提供的投票接口进行投票
    - CA读取根公钥，使用openssl接口对用户公钥的签名值进行校验
    - CA生成随机的AES加密所需要的参数（key，nonce，aad）
    - 使用用户公钥对AES参数进行加密
    - 调用TA的接口进行解密保存
    - TA使用用户公私钥对进行解密
    - 解密后TA调用GP的安全存储API保存AES参数
    - CA使用AES参数对投票数据进行加密，然后调用TA接口解密投票数据
    - TA使用AES参数将投票数据解密返回给TA
    - Web端获取投票结果保存到数据库中

## 使用依赖
CA和TA需要编译使用，CA需要使用openssl，Web服务需要依赖Python和sqlite3，构建和运行需要依赖的软件和最低版本：

| 软件     | 最低版本 |
| ----    | ----    |
| gcc     | 4.8.5   |
| cmake   | 2.8     |
| openssl | 1.1.1   |
| Python  | 3.7     |
| sqlite3 | 3.8.3   |

1. 确保环境上以安装机密计算SDK，需要同时安装 `kunpeng-sc` 和 `kunpeng-sc-devel` ，下载地址[机密计算SDK](https://mirrors.huaweicloud.com/kunpeng/archive/Kunpeng_SDK/itrustee/)
2. 确保tzdriver正常加载，执行 `lsmod | grep tzdriver` 查看
3. 确保守护进程正常启动，执行 `ps -ef | grep teecd` 查看

## 使用教程

**此项目代码和部署方式仅供参考，请勿在生产环境直接使用**

1. 获取代码

```
git clone https://github.com/kunpengcompute/devkitdemo.git
```

2. 切换到secret-vote demo对应的文件夹中

```
cd devkitdemo/sc-sdk/examples/secret-vote/
```

3. 将CA/secret_vote_ca.h中的TA_UUID修改为开发者申请证书的uuid

4. 创建Python虚拟环境

```
python3 -m venv venv
```

5. 激活虚拟环境

```
source venv/bin/activate
```

6. 编译CA

```
bash build/build_ca.sh
```

7. 将 ./TA/ 目录下的manifest.txt 文件替换成，申请开发者证书时使用的manifest.txt文件

8. 修改TA/config_cloud.ini中的开发者私钥和config文件路径

9. 编译TA

**TA会校验CA的可执行程序路径，需要修改TA/secret_vote_ta.h中的PYTHON3_PATH为虚拟环境中python3的绝对路径**

```
bash build/install_ta.sh
```

10. 安装运行DEMO需要的依赖

```
cd service
pip install -r requirements.txt
```

11. 初始化数据库表结构

```
python3 manage.py makemigrations
python3 manage.py migrate
```

12. 初始化数据内容

**TA会校验CA的可执行程序路径，需要使用Python3的绝对路径**

```
/path/to/python3 manage.py init_db
```

**此时数据库中默认会创建10个用户user_00-user_09，默认密码123456789**

13. 运行DEMO

**TA会校验CA的可执行程序路径，需要使用Python3的绝对路径**

```
/path/to/python3 manage.py runserver IP:PORT
```

使用浏览器访问：http://IP:PORT/static/index.html


## 前端代码构建运行

1. 第三方包下载
```
npm install
```

2. 编译并运行开发模式
```
npm run serve
```

3. 编译并打包生产模式，打的包默认放在ui的dist文件夹下
```
npm run build
```
