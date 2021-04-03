/*
 * @Author: Toxic Cloud
 * @Date: 2021-03-28 14:33:25
 * @LastEditTime: 2021-04-03 15:43:29
 * @LastEditors: Please set LastEditors
 * @Description: 日志线程池
 * @FilePath: /HttpServer/src/LogThreadPool.cpp
 */
#include "../include/Utils/LogThreadPool.hpp"
#include "../include/Utils/LogBuffer.hpp"

LogThreadPool::LogThreadPool(size_t num)
{
    m_thread_num = num;
    is_running = false;
    is_stop = true;
}

LogThreadPool::~LogThreadPool()
{
    // 如果日志线程还在运行就停止
    if (is_running)
    {
        this->Stop();
    }
    // 清空日志任务队列
    while (!m_log_queue.empty())
    {
        delete m_log_queue.front();
        m_log_queue.front() = nullptr;
        m_log_queue.pop();
    }
}

/**
 * @brief 停止日志线程池
 * 
 */
void LogThreadPool::Stop()
{
    std::unique_lock<std::mutex> ul(m_wait_mutex);
    is_running = false;
    ul.unlock();
    m_wait_cv.notify_all();
    m_task_cv.notify_all();
    // 循环释放线程池中线程内存
    std::for_each(m_thread_list.begin(), m_thread_list.end(), [](std::thread *th) { th->join(); });
    for (auto &item : m_thread_list)
    {
        delete item;
    }
}

/**
 * @brief 等待任务执行完成
 * 
 */
void LogThreadPool::Wait()
{
    if (m_log_queue.empty())
    {
        this->Stop();
    }
    else
    {
        std::unique_lock<std::mutex> ul(m_wait_mutex);
        is_stop = true;
        m_wait_cv.wait(ul);
        this->Stop();
    }
}

/**
 * @brief 开始线程池
 * 
 */
void LogThreadPool::Start()
{
    if (!is_stop)
    {
        return;
    }
    is_running = true;
    is_stop = false;

    for (size_t i = 0; i < m_thread_num; i++)
    {
        // 每个新线程都会去执行线程默认函数
        std::thread *t = new std::thread(std::bind(&LogThreadPool::DefaultFunc, this));
        m_thread_list.push_back(t);
    }
}

/**
 * @brief 从任务队列中获取任务
 * 
 * @return LogBuffer* 
 */
LogBuffer *LogThreadPool::GetLog()
{
    std::unique_lock<std::mutex> ul(m_task_mutex);
    // 如果任务队列为空就陷入沉睡，等待唤醒
    while (m_log_queue.empty() && is_running)
    {
        m_task_cv.wait(ul);
    }
    LogBuffer *log = nullptr;
    if (!m_log_queue.empty())
    {
        log = m_log_queue.front();
        m_log_queue.pop();
    }
    return log;
}

/**
 * @brief 向任务队列中添加任务
 * 
 * @param log : Log任务
 */
void LogThreadPool::Add(LogBuffer *log)
{
    if (!log)
    {
        return;
    }
    if (m_thread_list.size() == 0)
    {
        log->run();
        if (log->Status() == LogBuffer::LOG_DOWN || log->Status() == LogBuffer::LOG_ERROR)
        {
            delete log;
            log = nullptr;
        }
    }
    else
    {
        std::unique_lock<std::mutex> ul(m_task_mutex);
        m_log_queue.push(log);
        ul.unlock();
        // 唤醒等待的线程，处理这个任务
        m_task_cv.notify_one();
    }
}

/**
 * @brief 线程默认执行函数
 * 
 */
void LogThreadPool::DefaultFunc(void)
{
    while (is_running)
    {
        LogBuffer *log = this->GetLog();
        if (log)
        {
            log->run();
            // 如果Log状态为写入完成就释放任务内存
            if (log->Status() == LogBuffer::LOG_DOWN || log->Status() == LogBuffer::LOG_ERROR)
            {
                delete log;
                log = nullptr;
            }
        }
        if (m_log_queue.empty() || is_stop)
        {
            {
                std::unique_lock<std::mutex> ul(m_task_mutex);
                if (!is_stop)
                {
                    continue;
                }
                else
                {
                    is_stop = true;
                    is_running = false;
                }
            }
            m_task_cv.notify_all();
        }
    }
}