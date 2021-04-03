/*
 * @Author: Toxic Cloud
 * @Date: 2021-04-03 16:25:02
 * @LastEditTime: 2021-04-03 16:29:34
 * @LastEditors: Please set LastEditors
 * @Description: 这是一个静态服务器示例
 * @FilePath: /HttpServer/test/default.cpp
 */

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