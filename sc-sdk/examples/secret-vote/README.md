# secret-vote demo

## 介绍
secret-vote demo是使用鲲鹏机密计算特性开发的匿名投票系统的参考实现

[鲲鹏机密计算特性介绍](https://www.hikunpeng.com/zh/developer/boostkit/confidential-computing)

项目主要分为4个模块：
- CA Client Application，客户端应用程序，编译生成libsecret-vote-ca.so供Web服务调用
- TA Trusted Application，可信应用程序，运行在安全操作系统
- service Web服务，使用Python的Django框架开发的Web服务，提供REST API，需要调用CA编译生成的libsecret-vote-ca.so实现公私钥对的申请和投票数据的加解密
- ui 前端，使用vue和element-ui开发的用于投票和结果展示的界面

## 使用依赖
CA和TA需要编译使用，CA需要使用openssl，Web服务需要依赖Python和sqlite3，构建和运行需要依赖的软件和最低版本：

| 软件     | 最低版本 |
| ----    | ----    |
| gcc     | 4.8.5   |
| cmake   | 2.8     |
| openssl | 1.1.1   |
| Python  | 3.7     |
| sqlite3 | 3.8.3   |

1. 确保环境上以安装机密计算SDK，需要同时安装`kunpeng-sc`和`kunpeng-sc-devel`，下载地址[机密计算SDK](https://mirrors.huaweicloud.com/kunpeng/archive/Kunpeng_SDK/itrustee/)
2. 确保tzdriver正常加载，执行 `lsmod | grep tzdriver` 查看
3. 确保守护进程正常启动，执行 `ps -ef | grep teecd` 查看

## 使用教程

1. 获取代码

```
git clone https://github.com/kunpengcompute/devkitdemo.git
```

2. 切入到secret-vote demo对应的文件夹中

```
cd devkitdemo/sc-sdk/examples/secret-vote/
```

3. 将CA/secret_vote_ca.h中的TA_UUID修改为开发者申请证书的uuid

4. 编译CA

```
cd build
bash build_ca.sh
```

5. 将 ./TA/ 目录下的manifest.txt 文件替换成,申请开发者证书时使用的manifest.txt文件

6. 修改TA/config_cloud.ini中的开发者私钥和config文件路径

7. 编译TA

**TA会校验CA的可执行程序路径，需要修改TA/secret_vote_ta.h中的PYTHON3_PATH为虚拟环境中python3的绝对路径**

```
bash install_ta.sh
```

8. 创建Python虚拟环境

```
cd ..
python3 -m venv venv
```

9. 激活虚拟环境

```
source venv/bin/activate
```

10. 安装运行DEMO需要的依赖

```
cd service
pip install -r requirements.txt
```

11. 初始化数据库表结构

```
python manage.py makemigrations
python manage.py migrate
```

12. 初始化数据内容

**TA会校验CA的可执行程序路径，需要使用Python3的绝对路径**

```
/path/to/python3 manage.py init_db
```

**此时数据库中默认会创建10个用户user_00-user_09，默认密码123456789**

13. 运行DEMO

**这种方式启动的是Django的开发服务器，生产环境请使用其他方式部署**

**TA会校验CA的可执行程序路径，需要使用Python3的绝对路径**

```
/path/to/python3 manage.py runserver IP:PORT
```

使用浏览器访问：http://IP:PORT/static/index.html

