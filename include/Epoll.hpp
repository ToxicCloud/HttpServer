/*
 * @Author: Toxic Cloud
 * @Date: 2021-03-31 11:36:45
 * @LastEditTime: 2021-04-03 00:06:54
 * @LastEditors: Please set LastEditors
 * @Description: Epoll封装
 * @FilePath: /HttpServer/include/Epoll.hpp
 */

#pragma once

#include <sys/epoll.h>
#include "Socket.hpp"
#include "Task.hpp"
#include "TException.hpp"
#include "TaskThreadPool.hpp"
#include "Router.hpp"

class Epoll
{
private:
    // 任务线程池
    TaskThreadPool* m_task_pool;
    // 日志线程
    LogThreadPool* m_log_pool;
    // Epoll套接字管理句柄
    int m_epoll_fd;
    // 最大监听数量
    int max_listen_num;
    // 是否运行
    bool is_running;
    // 服务套接字
    Socket m_ser_sock;
    // 活动事件
    epoll_event* m_events;
    // router
    Router* m_router;
public:
    Epoll(int num = 256);
    virtual ~Epoll();
    // 设置服务监听套接字
    void SetServerSocket(int sock);
    // 设置任务线程池
    void SetTaskPool(TaskThreadPool* pool);
    // 设置日志线程
    void SetLogPool(LogThreadPool* pool);
    // 运行
    void Loop();
    // 停止
    void Stop();
    // 添加事件
    void AddEvent(int eve, int sock, bool is_et);
    void AddEvent(int eve, Task* task, bool is_et);
    // 删除事件
    void DelEvent(int eve, int sock);
    void DelEvent(int eve, Task* task);
    // 设置Router
    void SetRouter(Router* router);
    // 获取Router
    Router* GetRouter();
};
