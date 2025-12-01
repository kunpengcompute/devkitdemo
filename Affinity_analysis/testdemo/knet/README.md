### USE
#### 1.Compile
```
gcc test_posix.c -o test_posix -pthread
```
#### 2.Run

##### Dynamic library Mode
```
LD_PRELOAD=./librecord.so RECORD_CONF=config.txt RECORD_OUT_DIR=/home/demo/data/ ./test_posix
```
The librecord.so is in the compressed package of this tool: tracelib-collector-aarch64.tar.gz or tracelib-collector-x86_64.tar.gz.
Need run in the directory that contains these dynamic libraries.

##### Attach Mode
```
devkit advisor knet attach -p XXXX -i config.txt -o /home/demo/data/
```

#### 3.Result
```
ls /home/demo/data/
vim /home/demo/data/record.XXXXXXX
```

Check compatibility:
The configuration template file is in /path-to-Devkit-CLI/advisor/devkitplugins/affnity/tools/compileopt,templates/compat_conf.json

```
devkit advisor knet compat -d /home/demo/data -c /home/demo/compat_conf.json
```

For more information and usage examples, please visit: https://www.hikunpeng.com/document/detail/zh/kunpengdevps/userguide/cliuserguide/KunpengDevKitCli_0110.html