构建指导：
1、将java_perf_test工程下载到本地
2、在cmd控制台中，进入到java_perf_test目录
3、执行mvn clean install -DskipTests 命令进行构建(需安装maven)
4、构建成功后，可在构建日志中找到对应包路径
5、将构建成功的包上传到要部署的服务器，选择jdk11启动项目 java -Xlog:gc*=info,gc+heap=debug:file=/home/gc-%t.log -jar demo_test-0.0.1-SNAPSHOT.jar 启动项目
6、若存在端口占用情况，可自行修改application.yml文件端口配置
项目启动成功后：

调用如下链接触发日志：
内存溢出
http://ip:9118/gctest/testOOM
年轻代gc
http://ip:9118/gctest/young
系统gc
http://ip:9118/gctest/systemgc
大对象gc
http://ip:9118/gctest/bigdata
元空间gc
http://ip:9118/gctest/metaspace

热点函数调用：
http://ip:9118/test/testHotMethod/20
http://ip:9118/test/testHotMethodTwo/20