#ifndef THREADPOLL_H
#define THREADPOLL_H

#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <future>
#include <condition_variable>
#include <memory>
#include <stdexcept>

class ThreadPoll{
private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex tasks_mtx;
    std::condition_variable condition;
    bool stop;
public:
    ThreadPoll(int size);
    ~ThreadPoll();
    template<typename Func, typename... Args>
    auto add(Func&& func, Args&&... args) 
        -> std::future<typename std::result_of<Func(Args...)>::type>;
};

ThreadPoll::ThreadPoll(int size) : stop(false){
    for(int i = 0; i < size; ++i){
        threads.emplace_back([this]{
            for(;;){
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(tasks_mtx);
                    condition.wait(lock, [this]{
                        return stop || !tasks.empty();
                    });
                    if(stop && tasks.empty()) return;
                    task = std::move(tasks.front());
                    tasks.pop();
                }
                task();
            }
        });
    }
}

template<typename Func, typename... Args>
auto ThreadPoll::add(Func&& f, Args&&... args)-> std::future<typename std::result_of<Func(Args...)>::type>{
    using return_type = typename std::result_of<F(Args...)>::type;
    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );
    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex lock(tasks_mtx);
        if(stop)
            throw std::runtime_error("ThreadPoll is already stop, don't add task any more!");
        tasks.emplace([task](){(*task)();});
    }
    condition.notify_one();
    return res;
}

ThreadPoll::~ThreadPoll(){
    {
        std::unique_lock<std::mutex> lock(tasks_mtx);
        stop = true;
    }
    condition.notify_all();
    for(std::thread &thread : threads)
        thread.join();
}


#endif
