/*
 * @Author: Toxic Cloud
 * @Date: 2021-03-28 14:33:15
 * @LastEditTime: 2021-03-28 21:04:18
 * @LastEditors: Please set LastEditors
 * @Description: 日志线程池
 * @FilePath: /HttpServer/include/Utils/LogThreadPool.hpp
 */

#pragma once

#include <thread>
#include <condition_variable>
#include <mutex>
#include <vector>
#include <queue>
#include <functional>

// 前置声明一个类
class Log;
class LogBuffer;

class LogThreadPool
{
private:
    /* data */
    // 日志线程数量
    size_t m_thread_num;
    // 日志线程数组
    std::vector<std::thread*> m_thread_list;
    // Log缓冲队列
    std::queue<LogBuffer*> m_log_queue;
    // 线程是否运行
    bool is_running;
    // 线程池是否需要停止
    bool is_stop;
    // 工作锁
    std::mutex m_task_mutex;
    // 工作条件变量
    std::condition_variable m_task_cv;
    // 等待锁
    std::mutex m_wait_mutex;
    // 等待条件变量
    std::condition_variable m_wait_cv;
private:
    // 线程默认执行函数
    void DefaultFunc(void);
    // 获取缓冲区
    LogBuffer* GetLog();
    void Wait();
public:
    // 线程数量默认为7
    LogThreadPool(size_t num = 7);
    virtual ~LogThreadPool();
    void Start();
    void Stop();
    void Add(LogBuffer* log);
};

