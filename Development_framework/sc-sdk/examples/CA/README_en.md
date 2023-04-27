# **CA demo**

English | [简体中文](README.md)

## Introduction

The **CA demo** shows examples on the insecure side. Run the CA demo together with the TA demo.

## Dependencies

1. Ensure that `kunpeng-sc-1.3.0` and `kunpeng-sc-devel-1.3.0` of the same version have been installed in the environment. Download the [Confidential Computing SDK](https://mirrors.huaweicloud.com/kunpeng/archive/Kunpeng_SDK/itrustee/).
2. Run the `lsmod | grep tzdriver` command to check whether the tzdriver is properly loaded.
3. Run the  `ps -ef | grep teecd` command to check whether the daemon is started properly.

## Guidance

1. Obtain the code.

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. Switch to the project root path.

   ```shell
   cd ./devkitdemo/Development_framework/sc-sdk/examples/CA/
   ```

3. Install the CA demo.

   ```shell
   chmod +x ./install.sh
   ./install.sh
   ```

