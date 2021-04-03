/*
 * @Author: Toxic Cloud
 * @Date: 2021-03-28 21:21:29
 * @LastEditTime: 2021-04-03 16:10:22
 * @LastEditors: Please set LastEditors
 * @Description: 任务线程池
 * @FilePath: /HttpServer/src/TaskThreadPool.cpp
 */
#include "TaskThreadPool.hpp"

TaskThreadPool::TaskThreadPool(size_t num)
{
    m_thread_num = num;
    m_log_pool = nullptr;
    is_running = false;
    is_stop = true;
}

TaskThreadPool::~TaskThreadPool()
{
    m_log_pool = nullptr;
    if (is_running){
        this->Stop();
    }
    while (!m_task_queue.empty()){
        delete m_task_queue.front();
        m_task_queue.front() = nullptr;
        m_task_queue.pop();
    }
}

/**
 * @brief 设置日志线程池
 * 
 * @param pool : 日志线程池
 */
void TaskThreadPool::SetLogThreadPool(LogThreadPool* pool)
{
    m_log_pool = pool;
}

/**
 * @brief 停止任务线程池
 * 
 */
void TaskThreadPool::Stop()
{
    std::unique_lock<std::mutex> ul(m_wait_mutex);
    is_running = false;
    m_wait_cv.notify_all();
    m_task_cv.notify_all();
    std::for_each(m_thread_list.begin(), m_thread_list.end(), [](std::thread* th){th->join();});
    for (auto& item : m_thread_list){
        delete item;
    }
}

void TaskThreadPool::Wait()
{
    if (m_task_queue.empty()){
        this->Stop();
    }else{
        std::unique_lock<std::mutex> ul(m_wait_mutex);
        is_stop = true;
        m_wait_cv.wait(ul);
        Stop();
    }
}

/**
 * @brief 开始线程池
 * 
 */
void TaskThreadPool::Start()
{
    is_stop = false;
    is_running = true;
    for (size_t i = 0; i < m_thread_num; ++i)
    {
        std::thread* th = new std::thread(std::bind(&TaskThreadPool::DefaultFunc, this));
        m_thread_list.push_back(th);
    }
}

/**
 * @brief 从任务队列取任务
 * 
 * @return Task* 
 */
Task* TaskThreadPool::GetTask()
{
    std::unique_lock<std::mutex> ul(m_task_mutex);
    while (m_task_queue.empty() && is_running){
        m_task_cv.wait(ul);
    }
    Task* task = nullptr;
    if (!m_task_queue.empty() && is_running){
        task = m_task_queue.front();
        m_task_queue.pop();
    }
    return task;
}

/**
 * @brief 添加任务到任务
 * 
 * @param task 
 */
void TaskThreadPool::AddTask(Task* task)
{
    if (!task){
        return;
    }
    std::unique_lock<std::mutex> ul(m_task_mutex);
    m_task_queue.push(task);
    ul.unlock();
    m_task_cv.notify_one();
}

/**
 * @brief 线程默认执行函数
 * 
 */
void TaskThreadPool::DefaultFunc(void)
{
    while (is_running){
        Task* task = this->GetTask();
        if (task){
            // 执行任务并检查
            task->Run();
            if (task->GetStatus() == Task::TASK_DOWN){
                delete task;
            }
        }
        if (m_task_queue.empty() || is_stop){
            {
                std::unique_lock<std::mutex> ul(m_task_mutex);
                if (!is_stop){
                    continue;
                }else{
                    is_stop = true;
                    is_running = false;
                }
            }
            m_task_cv.notify_all();
        }
    }
}