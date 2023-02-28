# hello-world demo

English | [简体中文](README.md)

## Introduction
The hello-world demo is a reference implementation of hello-world developed using the Kunpeng Confidential Computing TrustZone Kit.

[Confidential Computing TrustZone Kit](https://www.hikunpeng.com/en/developer/boostkit/confidential-computing)

This demo is a simple secure computing application, including CA and TA, containing a basic secure computing application that needs to implement interfaces, showing how CA and TA applications transfer data.

## Dependencies

1. Ensure that `kunpeng-sc` and `kunpeng-sc-devel` have been installed in the environment. Download the [Confidential Computing SDK](https://mirrors.huaweicloud.com/kunpeng/archive/Kunpeng_SDK/itrustee/).
2. Run the `lsmod | grep tzdriver` command to check whether the tzdriver is properly loaded.
3. Run the  `ps -ef | grep teecd` command to check whether the daemon is started properly.

## Usage Guidance

1. Obtain the code.

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. Go to the project root directory.

   ```shell
   cd ./devkitdemo/sc-sdk/examples/hello_world/
   ```

3. Set the absolute paths of the developer private key and config file in `./TA/config_cloud.ini`.

   ```shell
   vim ./TA/config_cloud.ini
   # Set signKey to the absolute path of private_key.pem
   # signKey = /home/kunpeng/devkitdemo/sc-sdk/examples/hello_world/TA/TA_cert/private_key.pem
   # Set configPath to the absolute path of config
   # configPath = /home/kunpeng/devkitdemo/sc-sdk/examples/hello_world/TA/signed_config/config
   ```

4. Replace the `manifest.txt` file in the `./TA/` directory with the `manifest.txt` file used for applying for a developer certificate.

5. Install the TA demo.

   ```shell
   cd ./TA/
   ./install.sh
   ```

6. Install the CA demo.

   ```shell
   cd ../CA/
   ./install.sh
   ```

7. Run the demo.

   Run the demo:
   ```shell
   /vendor/bin/hello_world
   ```
