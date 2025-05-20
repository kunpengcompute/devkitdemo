#!/bin/bash

# 检查参数数量
if [ $# -ne 1 ]; then
    echo "usage: bash $0 [env|balance|imbalance|send|isend]"
    exit 1
fi

# 检查参数是否合法
valid_args=("env" "balance" "imbalance" "send" "isend")
is_valid=0
for valid_arg in "${valid_args[@]}"; do
    if [ "$1" == "$valid_arg" ]; then
        is_valid=1
        break
    fi
done

if [ $is_valid -eq 0 ]; then
    echo "fatal：args must be one of env, balance, imbalance, send, isend"
    exit 1
fi

# 处理 env 参数
if [ "$1" == "env" ]; then
    # 检查并添加 OpenMPI bin 路径
    if [[ ":$PATH:" != *":/opt/software/openmpi-4.1.5/bin:"* ]]; then
        export PATH="/opt/software/openmpi-4.1.5/bin:$PATH"
    fi

    # 检查并添加 OpenMPI lib 路径
    if [[ ":$LD_LIBRARY_PATH:" != *":/opt/software/openmpi-4.1.5/lib:"* ]]; then
        export LD_LIBRARY_PATH="/opt/software/openmpi-4.1.5/lib:$LD_LIBRARY_PATH"
    fi
fi

# 执行相应的命令
case "$1" in
    "balance")
        mpirun --allow-run-as-root -np 8 /opt/tunerdemo/DevKit-CLI-25.0.0-Linux-Kunpeng/devkit tuner hpc-perf -L detail --critical-path /home/hcj/task_balance
        ;;
    "imbalance")
        mpirun --allow-run-as-root -np 8 /opt/tunerdemo/DevKit-CLI-25.0.0-Linux-Kunpeng/devkit tuner hpc-perf -L detail --critical-path /home/hcj/task_imbalance
        ;;
    "send")
        mpirun --allow-run-as-root -np 4 /opt/tunerdemo/DevKit-CLI-25.0.0-Linux-Kunpeng/devkit tuner hpc-perf -L detail --critical-path /home/hcj/send_recv
        ;;
    "isend")
        mpirun --allow-run-as-root -np 4 /opt/tunerdemo/DevKit-CLI-25.0.0-Linux-Kunpeng/devkit tuner hpc-perf -L detail --critical-path /home/hcj/isend_irecv
        ;;
esac
