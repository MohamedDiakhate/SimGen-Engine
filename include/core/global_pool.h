#pragma once
#include "core/threadpool.h"
#include <thread>

class GlobalPool {
public: 
    // Return same pool instance, for global access
    static ThreadPool& get(){
        
        static ThreadPool instance(std::thread::hardware_concurrency());
        return instance;
    }
};