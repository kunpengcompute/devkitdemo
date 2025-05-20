#include "taskpool.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int TASK_DATA_TAG = 1;
    const int TIME_DATA_TAG = 2;
    
    if (rank == 0) {  // 通信节点
        std::vector<Task> allTasks;
	int task_num = (size - 1) * 10;
        for(int i = 1; i <= task_num; i++) {
            int cost;
            if (i <= 0.3 * task_num) {
                cost = 1;
            } else if (i <= 0.7 * task_num) {
                cost = 2;
            } else {
                cost = 5;
            }
            allTasks.emplace_back(i, cost);
        }

        int rank0_task_count = (size + 1) / 2;  // 向上取整的计算方式
        std::vector<Task> rank0Tasks;
        for(int i = task_num + 1; i <= task_num + rank0_task_count; i++) {
            rank0Tasks.emplace_back(i, 5);
        }

        // 为每个工作进程依次分配任务
        size_t currentTask = 0;
        while (currentTask < allTasks.size()) {
            // 遍历所有工作进程
            for (int worker = 1; worker < size && currentTask < allTasks.size(); worker++) {
                int task_data[2] = {allTasks[currentTask].id, allTasks[currentTask].cost};
                MPI_Send(task_data, 2, MPI_INT, worker, TASK_DATA_TAG, MPI_COMM_WORLD);
                currentTask++;
            }
        }
        
        // 发送结束标记给所有工作进程
        for (int worker = 1; worker < size; worker++) {
            int end_mark[2] = {-1, -1};
            MPI_Send(end_mark, 2, MPI_INT, worker, TASK_DATA_TAG, MPI_COMM_WORLD);
        }

        for(auto& task : rank0Tasks) {
            task.execute();
        }

        std::vector<double> process_times(size, 0.0);
        std::vector<int> task_counts(size, 0);
        
        for (int worker = 1; worker < size; worker++) {
            MPI_Status status;
            double worker_time;
            MPI_Recv(&worker_time, 1, MPI_DOUBLE, worker, TIME_DATA_TAG, 
                     MPI_COMM_WORLD, &status);
            process_times[worker] = worker_time;
        }

        std::cout << "\n任务执行统计：" << std::endl;
        for (int i = 1; i < size; i++) {
            std::cout << "进程 " << i << " 执行时间: " << process_times[i] 
                      << " 秒" << std::endl;
        }

        double max_time = *std::max_element(process_times.begin(), process_times.end());
        double min_time = *std::min_element(process_times.begin() + 1, process_times.end());
        double imbalance_ratio = (max_time - min_time) / max_time * 100;
        
    } else {  // 工作节点
        double start_time = MPI_Wtime();
        int tasks_completed = 0;
        
        while (true) {
            int task_data[2];
            MPI_Status status;
            
            MPI_Recv(task_data, 2, MPI_INT, 0, TASK_DATA_TAG, 
                     MPI_COMM_WORLD, &status);
            
            if (task_data[0] == -1) break;
            
            Task task(task_data[0], task_data[1]);
            std::cout << "进程 " << rank << " 执行任务 " << task.id 
                      << " (耗时: " << task.cost << "秒)" << std::endl;
            task.execute();
            tasks_completed++;
        }
        
        double end_time = MPI_Wtime();
        double execution_time = end_time - start_time;

        MPI_Send(&execution_time, 1, MPI_DOUBLE, 0, TIME_DATA_TAG, 
                 MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
