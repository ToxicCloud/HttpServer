/*
 * @Author: Toxic Cloud
 * @Date: 2021-03-31 11:36:55
 * @LastEditTime: 2021-04-03 15:50:07
 * @LastEditors: Please set LastEditors
 * @Description: Epoll监听封装
 * @FilePath: /HttpServer/src/Epoll.cpp
 */
#include "Epoll.hpp"

Epoll::Epoll(int num)
{
    if (num < 7)
    {
        max_listen_num = 7;
    }
    max_listen_num = num;
    is_running = false;
    m_epoll_fd = epoll_create(max_listen_num);
    if (m_epoll_fd < 0)
    {
        TException exp("创建epoll失败");
        throw exp;
    }
    m_events = new epoll_event[max_listen_num];
    m_router = nullptr;
    m_task_pool = nullptr;
    m_log_pool = nullptr;
}

Epoll::~Epoll()
{
    delete[] m_events;
    if (is_running)
    {
        Stop();
    }
}

/**
 * @brief 设置服务监听套接字
 * 
 * @param sock : 监听的套接字
 */
void Epoll::SetServerSocket(int sock)
{
    m_ser_sock = sock;
    this->AddEvent(EPOLLIN, m_ser_sock, false);
}

/**
 * @brief 设置日志线程池
 * 
 * @param pool ：日志线程池
 */
void Epoll::SetLogPool(LogThreadPool* pool)
{
    m_log_pool = pool;
}

/**
 * @brief  设置任务线程池
 * 
 * @param pool : 任务线程池
 */
void Epoll::SetTaskPool(TaskThreadPool *pool)
{
    m_task_pool = pool;
}

/**
 * @brief 添加事件
 * 
 * @param eve ：事件
 * @param sock ：事件套接字
 * @param is_et ：是否启用EPOLLET
 */
void Epoll::AddEvent(int eve, int sock, bool is_et)
{
    epoll_event event;
    event.data.fd = sock;
    event.events = eve;
    if (is_et)
    {
        event.events |= EPOLLET;
    }
    epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, sock, &event);
}

/**
 * @brief 添加事件
 * 
 * @param eve : 事件
 * @param task ：事件任务
 * @param is_et ：是否启用EPOLLET
 */
void Epoll::AddEvent(int eve, Task *task, bool is_et)
{
    epoll_event event;
    event.data.fd = task->GetSocket();
    event.events = eve;
    event.data.ptr = static_cast<void *>(task);
    if (is_et)
    {
        event.events |= EPOLLET;
    }
    epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, task->GetSocket(), &event);
}

/**
 * @brief 删除事件
 * 
 * @param eve ：事件
 * @param sock ：事件绑定的套接字
 */
void Epoll::DelEvent(int eve, int sock)
{
    epoll_event event;
    event.data.fd = sock;
    event.events = eve;
    epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, sock, &event);
}

/**
 * @brief 删除任务监听的事件
 * 
 * @param eve ：事件
 * @param task ：任务
 */
void Epoll::DelEvent(int eve, Task *task)
{
    epoll_event event;
    event.data.fd = task->GetSocket();
    event.data.ptr = static_cast<void *>(task);
    event.events = eve;
    epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, task->GetSocket(), &event);
}

/**
 * @brief Epoll循环
 * 
 */
void Epoll::Loop()
{
    is_running = true;
    while (is_running)
    {
        int active_num = epoll_wait(m_epoll_fd, m_events, max_listen_num, -1);
        if (active_num <= 0)
        {
            continue;
        }
        for (int i = 0; i < active_num; ++i)
        {
            Socket sock = m_events[i].data.fd;
            if (sock.Get() == m_ser_sock.Get())
            {
                sockaddr_in cli_addr;
                socklen_t cli_addr_len = sizeof(cli_addr);
                Socket cli_sock = accept(m_ser_sock, (sockaddr *)&cli_addr, &cli_addr_len);
                if (cli_sock.Get() <= 0)
                {
                    continue;
                }
                Task *task = new Task(this, cli_sock);
                task->SetStatus(Task::TASK_READING);
                this->AddEvent(EPOLLIN, cli_sock, true);
                task->SetLogPool(m_log_pool);
                m_task_pool->AddTask(task);
            }
            /* else if (sock.Get() != m_ser_sock.Get() && m_events[i].events & EPOLLIN){
                Task* task = nullptr;
                try{
                    task = m_task_map.at(sock.Get());
                }catch(const std::out_of_range& r){
                    task = nullptr;
                }
                if (task){
                    m_task_pool->AddTask(task);
                }
            } */
        }
    }
}

/**
 * @brief 停止Epoll循环
 * 
 */
void Epoll::Stop()
{
    is_running = false;
}

/**
 * @brief 设置Router
 * 
 * @param router ：Router
 */
void Epoll::SetRouter(Router *router)
{
    m_router = router;
}

/**
 * @brief 获取Router
 * 
 * @return Router* 
 */
Router* Epoll::GetRouter()
{
    return m_router;
}