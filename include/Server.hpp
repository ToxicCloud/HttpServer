/*
 * @Author: Toxic Cloud
 * @Date: 2021-04-02 23:34:15
 * @LastEditTime: 2021-04-03 16:00:22
 * @LastEditors: Please set LastEditors
 * @Description: Servre类
 * @FilePath: /HttpServer/include/Server.hpp
 */

#include "Epoll.hpp"
#include "Router.hpp"
#include "TaskThreadPool.hpp"
#include "LogBuffer.hpp"

class Server
{
public:
    // 监听回调
    typedef std::function<void (int port, sockaddr_in addr)> ListenCallBack;
private:
    Setting* m_setting;
    Epoll* m_epoll;
    Router* m_router;
    TaskThreadPool* task_pool;
    LogThreadPool* log_pool;
    Socket m_ser_sock;
    sockaddr_in ser_addr;
    int m_port;
    bool is_setting;
public:
    Server(int epoll_num = 256, int task_num = 27);
    virtual ~Server();

    // 初始化监听
    Server& LoadRouter(Router* router);
    // 开始监听
    Server& Listen(int port = 80, ListenCallBack call_back = nullptr);
    // 初始化Setting
    Server& InitSetting(Setting* setting);
private:
    // 初始化服务
    bool Init(int port = 80);
    // 正式开始
    void Start();
};