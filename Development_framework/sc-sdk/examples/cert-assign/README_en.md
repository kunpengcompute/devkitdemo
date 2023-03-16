# cert-assign demo

English | [简体中文](README.md)

## Introduction
The cert-assign demo is a reference implementation of the certificate issuance system developed using the Kunpeng Confidential Computing TrustZone Kit.

[Confidential Computing TrustZone Kit](https://www.hikunpeng.com/en/developer/boostkit/confidential-computing)

This demo is used to issue an X509 certificate to users. It creates a root certificate (based on RSA or SM2 algorithm) and signs the certificate request file submitted by the user to generate an X509 certificate. The private key of the root certificate is encrypted and saved in the trusted execution environment (TEE) to prevent the private key from disclosure. The root certificate generation and user certificate issuance are computed in the TEE, preventing the compute process from being attacked.

## Dependencies

1. Ensure that `kunpeng-sc-1.3.0` and `kunpeng-sc-devel-1.3.0` of the same version have been installed in the environment. Download the [Confidential Computing SDK](https://mirrors.huaweicloud.com/kunpeng/archive/Kunpeng_SDK/itrustee/).
2. Run the `lsmod | grep tzdriver` command to check whether the tzdriver is properly loaded.
3. Run the  `ps -ef | grep teecd` command to check whether the daemon is started properly.

## Usage Guidance

1. Obtain the code.

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. Go to the project root directory.

   ```shell
   cd ./devkitdemo/Development_framework/sc-sdk/examples/cert-assign/
   ```

3. Set the absolute paths of the developer private key and config file in `./TA/config_cloud.ini`.

   ```shell
   vim TA/config_cloud.ini
   # Change signKey to the absolute path of private_key.pem.
   # secSignKey = /usr/local/kunpeng-sc-devel/examples/cert-assign/TA/TA_cert/private_key.pem
   # Change configPath to the absolute path of config.
   # configPath = /usr/local/kunpeng-sc-devel/examples/cert-assign/TA/signed_config/config
   ```

4. Replace the `manifest.txt` file in the `./TA/` directory with the `manifest.txt` file used for applying for a developer certificate.

5. Install the TA demo. When compiling the TA, you need to statically link the OpenSSL encryption and decryption library applicable to the TEE. After `kunpeng-sc-devel-1.1.0` is installed, the OpenSSL encryption and decryption library applicable to the TEE is installed to `/usr/local/kunpeng-sc-devel/examples/cert-assign/lib/libcrypto.a`. For reference, see **Compiling and Generating the libcrypto.a File for TEE**.

   ```shell
   cd ./TA/
   ./install.sh
   ```

6. Change TA_UUID in `../CA/cert_assign_ca.h`**` to the UUID that the developer uses to apply for a certificate.

   ```shell
   vim ../CA/cert_assign_ca.h
       #define TA_UUID                "d17fa523-ab23-47fa-97b4-ede34acf748c"    // Developer Certificate UUID [Replace it with the actual one]
   ```

7. Install the CA demo.

   ```shell
   cd ../CA/
   ./install.sh
   ```

8. Run the demo.

   Default user name: **admin**

   Password: **admin**

   You can use OpenSSL to create a certificate request file and use this application to sign the certificate request file to generate an X509 certificate. For example:
   ```
   # Generate the private key
   openssl genrsa -out cert.key 2048
   # Generate a certificate request file 
   openssl req -new -sha256 -key cert.key -subj "/C=CN/CN=test" -out cert.csr
   ```

   - Generate a root certificate for the first run. Enter the common name and key algorithm (RSA or SM2) of the root certificate. After the root certificate is generated, the private key of the root certificate is encrypted and saved using the trusted storage function of the TEE. The root certificate is returned to the TA and saved to the `root_cert.pem` file.
   - After the root certificate is generated, you can run `show` to view the root certificate information.
   - Generate a certificate request file. Enter `sign` to use the root certificate to sign the certificate request file and generate an X509 certificate.
   - Delete the root certificate and private key stored in the TEE:  `rm -rf /var/itrustee/sec_storage_data/sec_storage_data/cert_assign`.

   Run the demo:
   ```shell
   /vendor/bin/cert-assign
   ```


## **Compiling and Generating the libcrypto.a File for the TEE**

1. Download the OpenSSL-1.1.1k code file.
```shell
wget https://github.com/openssl/openssl/archive/refs/tags/OpenSSL_1_1_1k.tar.gz
```

2. Decompress the file:
```shell
tar -zxf OpenSSL_1_1_1k.tar.gz
```

3. Add the code file to the patch applicable to the TEE. The patch is stored in the patch directory.
```shell
cd openssl-OpenSSL_1_1_1k
# Replace the patch path with the actual one
patch -p1 < /path/to/openssl_for_tee.patch
```

4. Perform compilation. Before compilation, install `kunpeng-sc` and `kunpeng-sc-devel`. After compilation is complete, `libcrypto.a` applicable to the TEE is generated.
```shell
./config no-sock no-shared CFLAGS="-DOPENSSL_RAND_TEE -DNO_SYSLOG \
   -DOPENSSL_NO_UI_CONSOLE \
   -DOPENSSL_NO_SECURE_MEMORY -DOPENSSL_NO_STDIO \
   -DOPENSSL_NO_STATIC_ENGINE -D__STDC_NO_ATOMICS__ \
   -O -w -fno-short-enums -fno-omit-frame-pointer -Wextra \
   -nostdinc -nodefaultlibs -march=armv8-a -Os -Wno-main -fPIC \
   -Wno-error=unused-parameter -Wno-error=unused-but-set-variable \
   -I/usr/include/itrustee_sdk/thirdparty/musl/libc \
   -I/usr/include/itrustee_sdk/thirdparty/musl/libc/arch/aarch64 \
   -I/usr/include/itrustee_sdk/thirdparty/musl/libc/arch/aarch64/bits \
   -I/usr/include/itrustee_sdk/thirdparty/musl/libc/arch/generic \
   -I/usr/include/itrustee_sdk/TA \
   -I/usr/include/itrustee_sdk/TA/huawei_ext"
make -j 4
```