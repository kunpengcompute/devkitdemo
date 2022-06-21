# **kunpeng sc sdk 构建说明**

## 介绍
本目录主要用于自动化构建kunpeng sc 发布包使用，目前支持在CentOS7.6, openEuler20.03 SP1, UOS 20 SP1 3个系统上进行发布包构建

## 目录说明
- patch: 主要存放对集成的sc SDK的适配修改
- script：主要存放自动化打包需要的相关脚本

## 使用说明
使用如下命令运行script目录下的package.sh脚本即可完成发布包的构建，生成的发布包存放在当前目录下的output（构建完成后自动创建）目录中
~~~
bash package.sh
~~~