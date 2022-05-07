CA demo

介绍
CA demo展示非安全侧示例，如果需要运行，请和TA demo配合使用

使用依赖
  1）确保已安装TEE SDK
  2）确保tzdriver正常加载，执行 lsmod | grep tzdriver
  3）确保守护进程正常启动，执行 ps -ef | grep teecd
  
使用教程
  1）安装CA demo
     ./install.sh
  
  若要运行demo请继续执行下面步骤：
  1）安装TA demo
     cd ../TA
	 ./install.sh {signed_config_path} {TA_cert_path}
  2）运行demo
     /vendor/bin/rsa_demo