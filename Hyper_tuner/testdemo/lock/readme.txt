针对提供的demo做以下说明：
1. demo要求gcc版本在7.3以上
2.若需要关联源码，建议编译时加上-g参数
如：gcc -g pthreadd_mutex.c -o  pthread_mutex -lpthread
3.在使用系统全景调优时，开启任务的同时，必须同时在后台运行应用，否则无法采集到应用信息
4.提供的demo编译运行时间在20s左右，建议调优时间设置大于30s；用户可通过源码修改循环次数，修改运行时间
5.提供的pthread_mutex_long.c为死锁demo，提供学习参考；编译pthead_mutex_long.c时，gcc参数加上- lm