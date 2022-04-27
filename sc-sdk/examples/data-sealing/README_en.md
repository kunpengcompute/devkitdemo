# **data-sealing demo**

English | [简体中文](README.md)

## Introduction

The **data-sealing demo** shows a code example for CAs and TAs that encrypt and store files using TEE secure storage.

## Dependencies

1. Ensure that kunpeng-sc-devel-1.0.0 has been installed.
2. Run the **lsmod | grep tzdriver** command to ensure that the tzdriver is properly loaded.
3. Run the **ps -ef | grep teecd** command to ensure that the daemon process is started properly.

## Guidance

1. Obtain the code.

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. Switch to the project root path.

   ```shell
   cd ./devkitdemo/devkitdemo-main/sc-sdk/examples/data-sealing/
   ```

3. Modify the absolute paths of the developer private key and config file in the **./TA/cloud/config_cloud.ini** file.

   ```shell
   vim ./TA/cloud/config_cloud.ini
   # Change signKey to the absolute path of private_key.pem.
   # signKey = /home/kunpeng/data-sealing/TA/cloud/TA_cert/private_key.pem
   # # Change configPath to the absolute path of config.
   # configPath = /home/kunpeng/data-sealing/TA/cloud/signed_config/config
   ```

4. Replace the **manifest.txt** file in the **./TA/** directory with the **manifest.txt** file used for applying for the
   developer certificate.

5. Install the TA demo.

   ```shell
   cd ./TA/
   chmod +x ./install.sh
   ./install.sh
   ```

6. Change **DATA_SEALING_TA_UUID** in .**./CA/data_sealing_ca.h** to the UUID of the developer certificate.

   ```shell
   vim ../CA/data_sealing_ca.h
   # #define DATA_SEALING_TA_UUID        "7824610d-0e54-4448-a905-86fffeeff327"    //Developer certificate UUID [Change it based on the site requirements.]
   ```

7. Install the CA demo.

   ```shell
   cd ../CA/
   chmod +x ./install.sh
   ./install.sh
   ```

8. Run the demo.

   ```shell
   /vendor/bin/data-sealing
   ```
