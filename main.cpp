#include <iostream>
#include <functional>
#include <chrono>
#include "ThreadPoll.h"

void function(int i, std::string str)
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << str << i << std::endl;
}

int main(int argc, char const *argv[])
{
    std::function<void(int, std::string)> p = function;
    ThreadPoll poll;
    for (int i = 0; i < 10; ++i)
    {
        poll.add(p, 3, std::string("hello"));
    }
    std::cout << "before join_all" << std::endl;
    poll.join_all();
    std::cout << "after join_all" << std::endl;
    return 0;
}
