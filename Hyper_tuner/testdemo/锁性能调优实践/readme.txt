针对提供的两demo做以下说明：
1. demo要求gcc版本在7.3以上，低版本的gcc编译pthread_atomic.c会报错
2.若需要关联源码，建议编译时加上-g参数
如：gcc -g pthreadd_mutex.c -o  pthread_mutex -lpthread
pthread_atomic 同理
注意：若无法关联源码，可查看编译二进制是否存在buildid。大部分编译器开启
buildid，小部分不开启。可以使用file命令(如file pthread_mutex)查看编译后的二进制文件是否存在
buildid。若不存在，建议加上-Wl,--build-id选项
如：gcc -Wl,--build-id -g pthreadd_mutex.c -o  pthread_mutex -lpthread
3.在使用系统热点函数监测锁热点时，在任务开启的同时，后台运行对应的demo,否则无法采集到锁的热点
4.用户可通过源码修改循环次数，修改运行时间(不同系统不同gcc版本略有差异)
5.pthread_atomic.c是pthread_mutex.c优化后的demo，采用原子锁的方式实现实现累加
6.建议web页面设置的运行时长大于应用本身运行时长，否则可能出现web页面无数据情况

