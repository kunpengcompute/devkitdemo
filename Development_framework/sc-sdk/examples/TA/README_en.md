# **TA demo**

English | [简体中文](README.md)

## Introduction

The **TA demo** shows the example on the security side. Run the TA demo together with the CA demo.

## Dependencies

1. Ensure that kunpeng-sc-devel has been installed.
2. Run the **lsmod | grep tzdriver** command to ensure that the tzdriver is properly loaded.
3. Run the **ps -ef | grep teecd** command to ensure that the daemon process is started properly.

## Guidance

1. Obtain the code.

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. Switch to the project root path.

   ```shell
   cd ./devkitdemo/sc-sdk/examples/TA/
   ```

3. Modify the absolute paths of the developer private key and config file in the **./cloud/config_cloud.ini** file.

   ```shell
   vim ./cloud/config_cloud.ini
   # Change signKey to the absolute path of private_key.pem.
   # signKey = /home/kunpeng/TA/cloud/TA_cert/private_key.pem
   # Change configPath to the absolute path of config.
   # configPath = /home/kunpeng/TA/cloud/signed_config/config
   ```

4. Replace the **manifest.txt** file in the **./** directory with the **manifest.txt** file used for applying for the
   developer certificate.

5. Install the TA demo.

   ```shell
   chmod +x ./install.sh
   ./install.sh
   ```
