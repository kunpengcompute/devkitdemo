# **RSA demo**

English | [简体中文](README.md)

## Introduction

The **RSA demo** shows examples of secure TA and CA communications.

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
   cd ./devkitdemo/sc-sdk/examples/RSA/
   ```

3. Modify the absolute paths of the developer private key and config file in the **./TA/cloud/config_cloud.ini** file.

   ```shell
   vim ./TA/cloud/config_cloud.ini
   # Change signKey to the absolute path of private_key.pem.
   # signKey = /home/RSA/TA/cloud/TA_cert/private_key.pem
   # # Change configPath to the absolute path of config.
   # configPath = /home/RSA/TA/cloud/signed_config/config
   ```

4. Replace the **manifest.txt** file in the **./TA/** directory with the **manifest.txt** file used for applying for the
   developer certificate.

5. Install the TA demo.

   ```shell
   cd ./TA/
   chmod +x ./install.sh
   ./install.sh
   ```

6. Modify **RSA_CRYPTO_uuid** and **RSA_CRYPTO_DEMO_TA_PATH** in **../CA/rsa_ca_demo.c**.

   ```shell
   vim ../CA/rsa_ca_demo.c
   # Change RSA_CRYPTO_uuid to the actual developer certificate UUID.
   # static const TEEC_UUID RSA_CRYPTO_uuid = {
   #    0xf68fd704, 0x6eb1, 0x4d14,
   #    { 0xb2, 0x18, 0x72, 0x28, 0x50, 0xeb, 0x3e, 0xf0 }
   # };
   # Change RSA_CRYPTO_DEMO_TA_PATH to /data/${uuid}.sec
   # #define RSA_CRYPTO_DEMO_TA_PATH        "/data/f68fd704-6eb1-4d14-b218-722850eb3ef0.sec"
   ```

7. Install the CA demo.

   ```shell
   cd ../CA/
   chmod +x ./install.sh
   ./install.sh
   ```

8. Run the demo.

   ```shell
   /vendor/bin/rsa_demo
   ```
