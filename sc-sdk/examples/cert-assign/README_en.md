# cert-assign demo

English | [简体中文](README.md)

## Introduction

The cert-assign demo is a reference implementation of the certificate issuance system developed using the Kunpeng confidential computing TrustZone Kit.

[Kunpeng BoostKit for Confidential Computing TrustZone Kit](https://www.hikunpeng.com/en/developer/boostkit/confidential-computing)

## Dependencies

1. Ensure that the confidential computing SDK is installed in the environment. You need to install both kunpeng-sc-1.0.1 and kunpeng-sc-devel-1.0.1. [Download at Confidential Computing SDK.](https://mirrors.huaweicloud.com/kunpeng/archive/Kunpeng_SDK/itrustee/)
2. Run the `lsmod | grep tzdriver` command to check whether the tzdriver is properly loaded.
3. Run the `ps -ef | grep teecd` command to check whether the daemon process is started properly.

## Guidance

1. Obtain the code.

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. Switch to the project root path.

   ```shell
   cd ./devkitdemo/sc-sdk/examples/cert-assign/
   ```

3. Modify the absolute paths of the developer private key and config file in the **./TA/cloud/config_cloud.ini** file.

   ```shell
   vim ./TA/cloud/config_cloud.ini
   # Change signKey to the absolute path of private_key.pem.
   # signKey = /home/kunpeng/devkitdemo/sc-sdk/examples/cert-assign/TA/TA_cert/private_key.pem
   # # Change configPath to the absolute path of config.
   # configPath = /home/kunpeng/devkitdemo/sc-sdk/examples/cert-assign/TA/signed_config/config
   ```

4. Replace the **manifest.txt** file in the **./TA/** directory with the **manifest.txt** file used for applying for the
   developer certificate.

5. Install the TA demo.

   ```shell
   cd ./TA/
   ./install.sh
   ```

6. Change **DATA_SEALING_TA_UUID** in .**./CA/cert_assign_ca.h** to the UUID of the developer certificate.

   ```shell
   vim ../CA/cert_assign_ca.h
  # #define TA_UUID                "d17fa523-ab23-47fa-97b4-ede34acf748c"    //Developer certificate UUID [Change it based on the site requirements.]
   ```

7. Install the CA demo.

   ```shell
   cd ../CA/
   ./install.sh
   ```

8. Run the demo.

   ```shell
   /vendor/bin/cert-assign
   ```
