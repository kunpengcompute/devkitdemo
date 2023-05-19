# java-hello-world demo

English | [简体中文](README.md)

## Dependencies

1. Ensure that `kunpeng-sc-2.0.0` and `kunpeng-sc-devel-2.0.0` of the same version have been installed in the environment. Download the [Confidential Computing SDK](https://mirrors.huaweicloud.com/kunpeng/archive/Kunpeng_SDK/itrustee/).
2. Run the `lsmod | grep tzdriver` command to check whether the tzdriver is properly loaded.
3. Run the  `ps -ef | grep teecd` command to check whether the daemon is started properly.

## Usage Guidance

1. Obtain the code.

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. Go to the project root directory.

   ```shell
   cd ./devkitdemo/Development_framework/sc-sdk/examples/java-hello-world/
   ```

3. Compile the demo.

    ```shell
    mkdir hellojava
    javac src/HelloWorld.java
    cp src/HelloWorld.class hellojava/
    ```

4. pack the demo.
    Put the developer certificate (ta_cert.der) and private key (private_key.pem) in the `/usr/local/kunpeng-sc-devel/utils/pack-app/pack_tools/` directory.
    ```shell
    cp -rf hellojava /usr/local/kunpeng-sc-devel/utils/pack-App/
    cd /usr/local/kunpeng-sc-devel/utils/pack-App/
    python3 pack.py hellojava
    ```

5. Install the demo.

    ```shell
    tee_teleport -c hellojava.sec
    ```

6. Run the demo.

    ```shell
    tee_teleport -r HelloWorld.class -i sessionID.txt
    ```

7. Obtain the log.

    ```shell
    tee_teleport -o output/tee.log -i sessionID.txt
    ```

8. Uninstall the demo.

    ```shell
    tee_teleport -e -i sessionID.txt
    ```
