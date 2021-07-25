#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
class ThreadPoll {
   private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex tasks_mtx;
    bool stop;

   public:
    ThreadPoll(int size);
    ~ThreadPoll();
    template <typename T, typename... Args>
    void add(std::function<T(Args...)> func, Args... args);
};

ThreadPoll::ThreadPoll(int size) : stop(false) {
    for (int i = 0; i < size; ++i) {
        threads.emplace_back(std::thread([this]() {
            while (true) {
                std::function<void()> func;
                std::unique_lock<std::mutex> tasks_lock(tasks_mtx);
                if (!tasks.empty()) {
                    func = tasks.front();
                    tasks.pop();
                    tasks_lock.unlock();
                    func();
                } else {
                    tasks_lock.unlock();
                }
                if (stop && tasks.empty()) return;
            }
        }));
    }
}

template <typename T, typename... Args>
void ThreadPoll::add(std::function<T(Args...)> func, Args... args) {
    std::function<void()> task = std::bind(func, args...);
    tasks.push(task);
}

ThreadPoll::~ThreadPoll() {
    stop = true;
    for (std::thread &th : threads) {
        if (th.joinable()) th.join();
    }
}
