构建指导：
1、将java_perf_test工程下载到本地
2、在cmd控制台中，进入到java_perf_test目录
3、执行mvn clean install -DskipTests 命令进行构建
4、构建成功后，可在构建日志中找到对应包路径
5、将构建成功的包上传到要部署的服务器，选择jdk11启动项目 java -Xlog:gc*=info,gc+heap=debug:file=/home/gc-%t.log -jar demo_test-0.0.1-SNAPSHOT.jar 启动项目