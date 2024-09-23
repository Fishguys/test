#include "PreComplier.h"

namespace ProducerConsumer {
    int maxTask = 100;
    int taskId = 0;

    std::queue<int> q;
    std::mutex mtx;
    std::condition_variable cv_producer;
    std::atomic<bool> quit = false;

    void producer(int id)
    {
        while (true)
        {
            int task = 0;

            {
                std::unique_lock<std::mutex> lock(mtx);
                if (taskId == maxTask)
                {
                    quit = true;
                    cv_producer.notify_all();
                    break;
                }

                task = taskId++;
                q.push(task);
            }

            cv_producer.notify_one();
            std::cout << "    - Producer threadID: " << std::this_thread::get_id() << ", taskId: " << taskId << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    void consumer(int id)
    {
        while (true)
        {
            int task;

            {
                std::unique_lock<std::mutex> lock(mtx);
                if (!q.empty())
                {
                    task = q.front();
                    q.pop();
                }
                else
                {
                    cv_producer.wait(lock, [] { return !q.empty() || quit; });
                    if (quit)
                    {
                        break;
                    }
                    task = q.front();
                    q.pop();
                }
            }

            std::cout << "- Consumer threadID: " << std::this_thread::get_id() << ",task: " << task << std::endl;
        }
    }


    void test() {
        const int numConsumers = 3;
        const int numProducers = 3;

        std::vector<std::thread> producerThreads;
        for (int i = 0; i < numProducers; ++i)
        {
            producerThreads.push_back(std::thread(producer, i));
        }

        std::vector<std::thread> consumerThreads;
        for (int i = 0; i < numConsumers; ++i)
        {
            consumerThreads.push_back(std::thread(consumer, i));
        }

        for (auto& t : producerThreads)
        {
            t.join();
        }

        for (auto& t : consumerThreads)
        {
            t.join();
        }
    }


    std::condition_variable condition;
    std::mutex mutex;
    std::atomic<bool> stop = false;
    int curId = 0;
    std::vector<char> Arphas{'A','B','C'};
    void printABC(int id)
    {
        while (true)
        {
            {
                std::unique_lock<std::mutex> lock(mutex);
                curId = curId % 3;
                condition.wait(lock, [=]() { return stop || curId == id; });
                std::cout << "print: " << std::this_thread::get_id() << " ,arpha: " << Arphas[curId] << std::endl;
                curId = curId + 1;
                condition.notify_all();
            }

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    void testABC() {
        const int numProducers = 3;

        std::vector<std::thread> producerThreads;
        for (int i = 0; i < numProducers; ++i)
        {
            producerThreads.push_back(std::thread(printABC, i));
        }

        for (auto& t : producerThreads)
        {
            t.join();
        }
    }
}