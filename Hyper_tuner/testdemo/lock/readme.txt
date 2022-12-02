针对提供的两demo做以下说明：
1. demo要求gcc版本在7.3以上，底版本的gcc编译pthread_atomic.c会报错
2.若需要关联源码，建议编译时加上-g参数
如：gcc -g pthreadd_mutex.c -o  pthread_mutex -lpthread
同理，pthread_atomic
3.在使用系统热点函数监测锁热点时，在任务开启的同时，后台运行对应的demo,否则无法采集到锁的热点
4.提供的demo编译运行时间在20s左右，建议调优时间设置大于30s；用户可通过源码修改循环次数，修改运行时间(不同系统不同gcc版本略有差异)
5.提供的pthread_mutex_long.c为死锁demo，提供学习参考；编译pthead_mutex_long.c时，gcc参数加上- lm
6.pthread_atomic.c是pthread_mutex.c优化后的demo，采用原子锁的方式实现实现累加
7.建议web页面设置的运行时长大于应用本身运行时长，否则可能出现web页面无数据情况