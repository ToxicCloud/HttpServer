/*
 * @Author: Toxic Cloud
 * @Date: 2021-03-28 11:43:29
 * @LastEditTime: 2021-04-03 16:28:26
 * @LastEditors: Please set LastEditors
 * @Description: 测试代码
 * @FilePath: /HttpServer/src/main.cpp
 */
#include <iostream>
#include "Server.hpp"

int main(void)
{
    Router router;
    Server server;
    Setting setting;

    setting.Set("WebHome", FileUtil::GetExecPath() + "/Webhome");
    
    router.On("/", "GET", [](Task& task){
        task.response->SenText(200, "这是主页");
    });
    router.On("/api/list", "GET", [](Task& task){
        task.response->NotFound();
    });
    server.InitSetting(&setting)
        .LoadRouter(&router).Listen(2021, [](int port, sockaddr_in addr){
            std::cout << "Listen on : http://127.0.0.1:" << port << std::endl;
        });
    return 0;
}