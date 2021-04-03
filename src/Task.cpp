/*
 * @Author: Toxic Cloud
 * @Date: 2021-03-29 11:15:02
 * @LastEditTime: 2021-04-03 16:33:23
 * @LastEditors: Please set LastEditors
 * @Description: 任务封装
 * @FilePath: /HttpServer/src/Task.cpp
 */
#include "Task.hpp"
#include "Epoll.hpp"

Task::Task(Epoll *epoll, const Socket &sock)
{
    m_epoll = epoll;
    m_sock = sock.Get();
    request = new Request(m_sock.Get());
    response = new Response(m_sock.Get());
    m_status = TASK_WAITING;
    log_pool = nullptr;
}

Task::~Task()
{
    delete request;
    delete response;
}

/**
 * @brief 设置日志线程池
 * 
 * @param pool 
 */
void Task::SetLogPool(LogThreadPool* pool)
{
    log_pool = pool;
}

/**
 * @brief 设置任务状态
 * 
 * @param status ：状态
 */
void Task::SetStatus(Task::Task_Status status)
{
    m_status = status;
}

/**
 * @brief 获取任务状态
 * 
 * @return Task::Task_Status 
 */
Task::Task_Status Task::GetStatus() const
{
    return m_status;
}

/**
 * @brief 读取任务
 * 
 */
void Task::ReadTask()
{
    try{
        request->ParseFromSocket(m_sock);
    }
    catch(TException& e)
    {
        std::cout << e.what() << '\n';
        m_sock.Close();
        m_status = TASK_DOWN;
        return;
    }
    m_epoll->DelEvent(EPOLLIN, this);
    m_status = TASK_WRITING;
}

/**
 * @brief 写任务
 * 
 */
void Task::WriteTask()
{
    response->InitHeader(*request);
    Router* router = m_epoll->GetRouter();
    if (router){
        router->Patch(request->GetHead("Path"), request->GetHead("Method"))(*this);
    }else{
        m_sock.Close();
    }
    m_status = TASK_DOWN;
}

/**
 * @brief 执行任务
 * 
 */
void Task::Run()
{
    if (m_status == TASK_READING){
        ReadTask();
    }
    if (m_status == TASK_WRITING){
        WriteTask();
    }
}

/**
 * @brief 获取Task对应套接字
 * 
 * @return Socket& 
 */
Socket& Task::GetSocket()
{
    return m_sock;
}