/*
 * @Author: Toxic Cloud
 * @Date: 2021-04-03 16:20:39
 * @LastEditTime: 2021-04-03 16:24:42
 * @LastEditors: Please set LastEditors
 * @Description: 这是使用日志的测试案例
 * @FilePath: /HttpServer/test/log.cpp
 */

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
