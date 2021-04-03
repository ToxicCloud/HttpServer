/*
 * @Author: Toxic Cloud
 * @Date: 2021-04-02 23:34:22
 * @LastEditTime: 2021-04-03 16:04:34
 * @LastEditors: Please set LastEditors
 * @Description: Server类
 * @FilePath: /HttpServer/src/Server.cpp
 */
#include "Server.hpp"

Server::Server(int epoll_max_num, int task_pool_num)
{
    if (epoll_max_num < 7)
    {
        epoll_max_num = 7;
    }
    if (task_pool_num < 7)
    {
        task_pool_num = 7;
    }
    m_epoll = new Epoll(epoll_max_num);
    task_pool = new TaskThreadPool(task_pool_num);
    log_pool = new LogThreadPool();
    m_router = nullptr;
    m_port = 80;
    is_setting = false;
}

Server::~Server()
{
    m_epoll->Stop();
    task_pool->Stop();
    log_pool->Stop();
    delete m_epoll;
    delete task_pool;
    delete log_pool;
}

/**
 * @brief 初始化监听
 * 
 * @param router : Router类
 * @return Server& 
 */
Server& Server::LoadRouter(Router *router)
{
    if (router)
    {
        m_router = router;
    }
    return *this;
}

/**
 * @brief 正式开始监听
 * 
 * @param port : 监听端口
 * @param call_back ：监听完毕后的回调
 * @return Server& 
 */
Server& Server::Listen(int port, Server::ListenCallBack call_back)
{
    if (this->Init(port)){
        call_back(port, ser_addr);
        this->Start();
    }else{
        call_back = [](int port, sockaddr_in addr) {
            std::cout << "Can't Listen on http://127.0.0.1:" + port << std::endl;
        };
        call_back(port, ser_addr);
    }
    return *this;
}

/**
 * @brief 正式开始函数
 * 
 */
void Server::Start()
{
    m_epoll->SetServerSocket(m_ser_sock);
    m_epoll->SetRouter(m_router);
    m_epoll->SetLogPool(log_pool);
    m_epoll->SetTaskPool(task_pool);
    m_router->InitSetting(m_setting);
    task_pool->SetLogThreadPool(log_pool);
    log_pool->Start();
    task_pool->Start();
    m_epoll->Loop();
}

/**
 * @brief 初始化一些服务端信息
 * 
 * @param port 
 * @return true 
 * @return false 
 */
bool Server::Init(int port)
{
    m_ser_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (m_ser_sock < 0)
    {
        return false;
    }
    ser_addr.sin_addr.s_addr = ntohl(INADDR_ANY);
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(port);

    int opt = 1;
    setsockopt(m_ser_sock, SOL_SOCKET, SO_REUSEADDR, static_cast<void *>(&opt), sizeof(opt));

    if ((bind(m_ser_sock, (sockaddr *)&ser_addr, sizeof(ser_addr))) < 0)
    {
        return false;
    }

    if ((listen(m_ser_sock, 7)) < 0)
    {
        return false;
    }
    return true;
}

/**
 * @brief 设置Setting
 * 
 * @param setting 
 * @return Server& 
 */
Server& Server::InitSetting(Setting* setting)
{
    m_setting = setting;
}