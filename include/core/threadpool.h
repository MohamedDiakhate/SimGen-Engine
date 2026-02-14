#pragma once
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

class ThreadPool
{

public:
    ThreadPool(size_t num_threads)
    {

        for (size_t i = 0; i < num_threads; i++)
        {
            threads_.emplace_back([this] {
                while (true){
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(queue_mutex);
                        cv.wait(lock, [this] { return stop || !tasks.empty() ;});
                        
                        if (stop && tasks.empty()) return;

                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    task();
                    
                }
            });
        }
    }

    ~ThreadPool()
    {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            stop = true;
        }
        cv.notify_all();

        for (auto& thread : threads_){
            thread.join();
        }

    }

    void enqueue(std::function<void()> task)
    {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            tasks.emplace(std::move(task));
        }
        cv.notify_one();
    }

private:
    std::vector<std::thread> threads_;

    std::queue<std::function<void()>> tasks;

    std::mutex queue_mutex;

    // sleep/wake threads
    std::condition_variable cv;

    // stop all threads from working
    bool stop = false;
    
};