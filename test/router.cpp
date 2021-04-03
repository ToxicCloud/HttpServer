/*
 * @Author: Toxic Cloud
 * @Date: 2021-04-03 16:13:57
 * @LastEditTime: 2021-04-03 16:21:26
 * @LastEditors: Please set LastEditors
 * @Description: 使用Router的案例
 * @FilePath: /HttpServer/test/router.cpp
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
