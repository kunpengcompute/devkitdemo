#pragma once
#include <mpi.h>
#include <vector>
#include <chrono>
#include <thread>

class Task {
public:
    int id;
    int cost;
    
    Task(int _id, int _cost) : id(_id), cost(_cost) {}
    
    void execute() {
        std::this_thread::sleep_for(std::chrono::seconds(cost));
    }
};
