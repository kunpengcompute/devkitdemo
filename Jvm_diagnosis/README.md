# jvm_demo
JVM波动检测工具运行demo。

#### 介绍
常见退优化原因：类加载、方法多态化、反射调用、异常分支、动态代理等

Demo1：Main.java

A.foo() 在JIT编译时最初被认为只有一个实现，可以安全内联。但运行过程中通过Class.forName("B")动态加载了子类并重写了foo()，因此会触发退优化。

Demo2：App.java, SerializationJsonTestTwo.java

序列化与反序列化函数执行时，由于多个文件的json格式不同，在处理不同类型的数据结构时产生退优化。

#### 使用
1.运行demo准备
Demo1：
编译：
```
javac Main.java
```
运行：
```
java Main
```

Demo2:
下载json数据和jar包，json文件放置在JvmDiagnosis/jsonData文件夹中，jar包放置在当前JvmDiagnosis文件夹中

5MB.json：https://microsoftedge.github.io/Demos/json-dummy-data/

user_1k.json, user_10k.json, user100k.json：https://jsoneditoronline.org/indepth/datasets/json-file-example/

jar包地址：https://repo1.maven.org/maven2/org/json/json/20190722/json-20190722.jar

编译：
```
javac -classpath .:json-20190722.jar App.java SerializationJsonTestTwo.java
```
运行：
```
java -classpath .:json-20190722.jar App
```

Demo1采集结果不稳定，推荐使用Demo2采集jfr文件

2.修改配置文件
修改kunpeng_devkit.cfg文件，<jvm.DeoptimizationStatistics>中<enabled>项默认为false，需修改为true启动监控。

其他配置项如采集间隔、采集时间和退优化采集阈值等参数需根据实际监控程序修改。

针对demo程序，需修改推荐参数配置
```
jvm.DeoptimizationStatistics:
    enabled: true
    duration: 30 s
    jitTtraceThreshold: 0.5
```

3.监控终端执行：
```
devkit diag jvm-jitter -a monitor -e jit.deoptimization -p XXXX
```
执行监控命令后，默认在程序执行的目录下生成devkit_odin.properties文件，修改参数为False开启监控。

由于Demo执行时间较短，推荐第一次监控生成devkit_odin.properties文件修改后再次测试。

更多参数和使用说明，请查看: https://www.hikunpeng.com/document/detail/zh/kunpengdevps/userguide/cliuserguide/KunpengDevKitCli_0131.html