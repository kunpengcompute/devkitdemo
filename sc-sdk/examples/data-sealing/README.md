# data-sealing demo

## 介绍
data-sealing demo展示使用TEE安全存储对文件进行加密保存的CA和TA的代码示例

## 使用依赖
  1. 确保已安装kunpeng-sc-devel-1.0.0
  2. 确保tzdriver正常加载，执行 lsmod | grep tzdriver
  3. 确保守护进程正常启动，执行 ps -ef | grep teecd
  
## 使用教程
  1. 修改TA/cloud/config_cloud.ini中的开发者私钥和config的绝对路径
  
    - 修改signKey为private_key.pem绝对路径
    - 修改configPath为config绝对路径

  2. 将TA/manifest.txt修改为开发者申请证书的manifest.txt

  3. 安装TA demo

    cd TA && ./install.sh

  4. 将CA/data-sealing-ca.h中的DATA_SEALING_TA_UUID修改为开发者申请证书的uuid

  5. 安装CA demo

    cd CA && ./install.sh

  6. 运行demo
  
    /vendor/bin/data-sealing
