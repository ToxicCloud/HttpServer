# HttpServer
一个简单的服务器，支持http协议，主要功能就是静态网页返回以及监听特定路径并控制要返回的数据。它比较适合一些初入服务端C、C++编程伙伴们学习~<br>
  1. 它体积很小不到2000行，主要实现是利用Epoll监听客户端连接，然后将连接封装成一个任务，再交由线程池中的线程进行处理。
  2. 虽然采用C++实现，但是它并不复杂，没有使用大量新特性，只使用了C++11部分特性
  3. 内部每个函数都有部分注释，实现不复杂
  
当然，这个项目主要是我学习时写的一个小项目，避免不了性能不高，有少量bug等
  1. 项目中没有使用定时器去踢除无用连接，后续会加入实现
  2. http解析只实现了get请求的解析，post请求解析会丢掉请求体，后续会改进
  3. 性能不是很高
  
 使用[webbench](https://github.com/EZLippi/WebBench)压力测试结果:
![压力测试例子](https://github.com/ToxicCloud/HttpServer/blob/master/test/webbench.png)

他的使用很简单，你只需要进入到源码根目录然后执行make即可
```shell
cd source_code_root
make && make run
// make run
```
 
 测试例子:
 
 [静态网页例子](https://github.com/ToxicCloud/HttpServer/blob/master/test/default.cpp)
 ```cpp
#include "Server.hpp"

int main(void)
{
    Setting setting;
    Router router;
    //  第一个参数为Epoll最多监听套接字数量， 第二个参数是任务线程池线程数量
    Server server(256, 77);
    
    setting.Set("WebHome", FileUtil::GetExecPath() + "/WebHome");

    server.InitSetting(&setting)
        .LoadRouter(&router)
        .Listen(2021);
    return 0;
}
```
[拦截指定路径](https://github.com/ToxicCloud/HttpServer/blob/master/test/router.cpp)
```cpp
int main(void)
{
    Setting setting;
    Router router;
    Server server;

    setting.Set("WebHome", FileUtil::GetExecPath() + "/WebHome");

    // Router监听
    router.On("/api/list", "GET", [](Task& task){
        task.response->SenText(200, "这是列表接口");
    });

    router.On("/", "GET", [](Task& task){
        task.response->Error("这是一个错误");
    });

    server.InitSetting(&setting)
        .LoadRouter(&router)
        .Listen(2021, [](int port, sockaddr_in addr){
            std::cout << "Listen on http://127.0.0.1:" + port << std::endl;
        });
    return 0;
}
```
[日志使用](https://github.com/ToxicCloud/HttpServer/blob/master/test/log.cpp)
```cpp
#include "Server.hpp"

int main(void)
{
    Setting setting;
    Router router;
    Server server;

    setting.Set("WebHome", FileUtil::GetExecPath() + "/WebHome");

    // Router监听
    router.On("/api/list", "GET", [](Task& task){
        task.response->SenText(200, "这是列表接口");
        Log log(task.log_pool);
        Path path = FileUtil::GetExecPath() + "/Log.txt";
        log.i(path, "这是一个普通日志记录");
        log.r(path, "这是一个警告日志记录");
    });

    router.On("/", "GET", [](Task& task){
        task.response->Error("这是一个错误");
        Log log(task.log_pool);
        Path path = FileUtil::GetExecPath() + "/Log.txt";
        log.e(path, task.response->path() + "Not Found");
    });

    server.InitSetting(&setting)
        .LoadRouter(&router)
        .Listen(2021, [](int port, sockaddr_in addr){
            std::cout << "Listen on http://127.0.0.1:" + port << std::endl;
        });
    return 0;
}
```


最后，如果你在研究或者使用中发现任何问题，欢迎随时骚扰我。
QQ Email: 2938384958@qq.com
Google Email: tw2938384958@gmail.com

祝你好运~~
