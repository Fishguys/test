#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#include "Singleton.h"

namespace Thread_Pool {
    class ThreadPool : public Singleton<ThreadPool> {
        friend class Singleton<ThreadPool>;
    public:
        template<class F, class... Args>
        auto enqueue(int priority, F&& f, Args&&... args)
            -> std::future<typename std::invoke_result<F, Args...>::type>;

    private:
        ThreadPool(size_t num_threads
            = std::thread::hardware_concurrency());
        void run();
        ~ThreadPool();
    private:
        std::vector<std::thread> workers;
        std::priority_queue<std::pair<int, std::function<void()>>, std::vector<std::pair<int, std::function<void()>>>, decltype([](const auto& a, const auto& b) {
            return a.first > b.first;
            })> tasks;
        std::mutex queueMutex;
        std::condition_variable condition;
        std::atomic<bool> stopRun{false};
    };

    void testThreadPool();
}
