#include <vector>
#include <thread>
#include <functional>

class ThreadPoll
{
private:
    std::vector<std::thread> threads;
public:
    ThreadPoll();
    ~ThreadPoll();
    template <typename T, typename... Args>
    void add(std::function<T(Args...)>, Args...);
    void join_all();
};
ThreadPoll::ThreadPoll()
{
    std::vector<std::thread> threads;
}
ThreadPoll::~ThreadPoll()
{
    join_all();
}

template <typename T, typename... Args>
void ThreadPoll::add(std::function<T(Args...)> func, Args... args)
{
    threads.push_back(std::thread(func, args...));
}

void ThreadPoll::join_all()
{
    for (std::thread &th : threads)
    {
        if (th.joinable())
            th.join();
    }
}
