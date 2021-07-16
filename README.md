# ThreadPoll

C++线程池

## api

```c++
#include "ThreadPoll.h"

ThreadPoll poll;    //创建一个线程池

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
poll.add(func, 1, 'a', std::string("Hello World!"));    //讲func指向的函数添加到线程池

poll.join_all();    //将线程池中所有线程join()，即等待所有线程运行结束再执行主线程下面的操作
```
