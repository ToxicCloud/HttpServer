/*
 * @Author: Toxic Cloud
 * @Date: 2021-03-28 21:21:20
 * @LastEditTime: 2021-03-28 21:47:27
 * @LastEditors: Please set LastEditors
 * @Description: 任务线程池
 * @FilePath: /HttpServer/include/TaskThreadPool.hpp
 */
#pragma once

#include "Utils/LogBuffer.hpp"
#include "Task.hpp"
#include <algorithm>

class TaskThreadPool
{
private:
    // 线程数量
    size_t m_thread_num;
    // 线程数组
    std::vector<std::thread *> m_thread_list;
    // 任务队列
    std::queue<Task *> m_task_queue;
    // 线程池是否在运行
    bool is_running;
    // 线程池是否停止
    bool is_stop;
    // 工作条件变量与互斥锁
    std::mutex m_task_mutex;
    std::condition_variable m_task_cv;
    // 等待条件变量与互斥锁
    std::mutex m_wait_mutex;
    std::condition_variable m_wait_cv;
    // 日志记录线程池
    LogThreadPool* m_log_pool;
private:
    // 线程默认执行函数
    void DefaultFunc(void);
    // 获取任务
    Task* GetTask();
    // 等待任务完成
    void Wait();
public:
    // 默认线程数量为77
    TaskThreadPool(size_t num = 77);
    virtual ~TaskThreadPool();

    //设置日志线程池
    void SetLogThreadPool(LogThreadPool* pool);
    // 开始线程池运作
    void Start();
    // 停止线程池
    void Stop();
    // 添加任务
    void AddTask(Task* task);
};