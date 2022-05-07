RSA demo

介绍
RSA demo展示TA与CA安全通信示例

使用依赖
  1）确保已安装TEE SDK
  2）确保tzdriver正常加载，执行 lsmod | grep tzdriver
  3）确保守护进程正常启动，执行 ps -ef | grep teecd
  
使用教程
  1）适配修改
    cd TA
	vim ./cloud/CMakeList.txt
	修改iTrustee_SDK_PATH为TEE SDK安装路径；
	vim ./cloud/config_cloud.ini
	修改signKey为private_key.pem绝对路径；
	修改configPath为config绝对路径；
  2）安装TA demo
    ./install.sh
  3）安装CA demo
    cd CA
	./install.sh
  4）运行demo
    /vendor/bin/rsa_demo