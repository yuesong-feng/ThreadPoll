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
    ThreadPoll poll(10);
    for (int i = 0; i < 10; ++i)
    {
        poll.add(p, 3, std::string("hello"));
    }
    return 0;
}
