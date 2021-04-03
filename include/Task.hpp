/*
 * @Author: Toxic Cloud
 * @Date: 2021-03-28 21:25:52
 * @LastEditTime: 2021-04-03 00:03:26
 * @LastEditors: Please set LastEditors
 * @Description: 任务
 * @FilePath: /HttpServer/include/Utils/Task.hpp
 */

#pragma once
#include "Request.hpp"
#include "Socket.hpp"
#include "Response.hpp"

class Epoll;

class Task
{
public:
    // 任务状态
    enum Task_Status{
        // 等待
        TASK_WAITING = 0x01,
        // 读取
        TASK_READING = 0x02,
        // 写
        TASK_WRITING = 0x03,
        // 完成
        TASK_DOWN = 0x04
    };
    // HTTP请求与响应
    Request* request;
    Response* response;
    LogThreadPool* log_pool;
private:
    // 指向Epoll监听的指针
    Epoll* m_epoll;
    // 任务的套接字
    Socket m_sock;
    // 任务当前状态
    Task_Status m_status;
public:
    Task(Epoll* epoll, const Socket& sock);
    virtual ~Task();
    // 获取Task对应Socket
    Socket& GetSocket();
    // 正式执行任务
    void Run();
    // 设置任务状态
    void SetStatus(Task_Status status);
    // 获取任务状态
    Task_Status GetStatus() const;
    // 设置日志线程
    void SetLogPool(LogThreadPool* pool);
private:
    // 读取任务
    void ReadTask();
    // 写任务
    void WriteTask();
};
