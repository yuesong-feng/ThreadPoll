# ThreadPoll

C++线程池

## api

```c++
#include "ThreadPoll.h"

ThreadPoll poll(int size);    //创建一个线程池，传入size表示工作线程数量

/*
    int myfunction(int i, char c, std::string str){
        ......
    }
*/
std::function<int(int, char, std::string)> func = my_function;    //func可以理解为C中的函数指针，指向my_function函数，可以通过func调用

/*
    add函数定义，支持任意返回类型、任意参数类型、任意参数个数
    template <typename T, typename... Args>
    void add(std::function<T(Args...)>, Args...);
*/
poll.add(func, 1, 'a', std::string("Hello World!"));    //将func指向的函数添加到线程池，函数参数按顺序传递

```
