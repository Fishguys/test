#include "PreComplier.h"
#include "ThreadPool.h"

namespace Thread_Pool
{
    // the constructor just launches some amount of workers
    inline ThreadPool::ThreadPool(size_t threads)
    {
        for (size_t i = 0; i < threads; ++i)
            workers.emplace_back(&ThreadPool::run, this);
    }

    template<class F, class... Args>
    auto ThreadPool::enqueue(int priority, F&& f, Args&&... args)
        ->std::future<typename std::invoke_result<F, Args...>::type>
    {

        using return_type = typename std::invoke_result<F, Args...>::type;
        auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            tasks.emplace(priority, [task]() { (*task)(); });
        }
        condition.notify_one();
        return res;
    }

    void ThreadPool::run()
    {
        while (!stopRun) {
            std::function<void()> task;
            if (!tasks.empty())
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                const auto& iter = tasks.top();
                task = iter.second;

                tasks.pop();
            }else
            {
                
                std::unique_lock<std::mutex> lock(queueMutex);
                condition.wait(lock, [this] { return stopRun || !tasks.empty(); });

                if (stopRun)
                    break;

                const auto& iter = tasks.top();
                task = iter.second;
                
                tasks.pop();
            }
            
            if(task)
                task();
        }
    }

    //// the destructor joins all threads
    ThreadPool::~ThreadPool()
    {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            stopRun = true;
        }
        condition.notify_all();
        for (std::thread& worker : workers)
            worker.join();
    }

    void result(int id, int priority)
    {
        std::cout << "Task End" << id << std::endl;
    }

    using callback = std::function<void(int, int)>;
    void subtask(int id, int priority, callback cb)
    {
        auto now = std::chrono::system_clock::now();
        std::cout << "Task Start " << id << " with priority " << priority << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        cb(id, priority);
    }

    void testThreadPool()
    {
        for (int i = 0; i < 10; ++i) {
            ThreadPool::Instance().enqueue(i % 3, subtask, i, i % 3, result);
        }
    }

}
