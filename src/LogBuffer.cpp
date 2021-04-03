/*
 * @Author: Toxic Cloud
 * @Date: 2021-03-28 14:07:53
 * @LastEditTime: 2021-04-03 15:36:07
 * @LastEditors: Please set LastEditors
 * @Description: 日志记录
 * @FilePath: /HttpServer/src/LogBuffer.cpp
 */

#include "LogBuffer.hpp"


LogBuffer::LogBuffer() : m_path(""), m_status(LOG_WAITING)
{
}

LogBuffer::~LogBuffer()
{
}

void LogBuffer::SetPath(const Path& path)
{
    m_path = path;
}

LogBuffer::Log_Status LogBuffer::Status() const
{
    return m_status;
}

void LogBuffer::Buffer(const std::string& str)
{
    m_buffer << str;
}

void LogBuffer::run()
{
    if (!m_path.Exist()){
        if (!m_path.Create()){
            m_status = LOG_ERROR;
            return;
        }
    }
    std::ofstream of(m_path.C_Str());
    if (!of.is_open()){
        m_status = LOG_ERROR;
        return;
    }
    of << m_buffer.rdbuf();
    m_buffer.clear();
    of.close();
    m_status = LOG_DOWN;
}


/* ---------------------------------------------------------------------------------------------------------------------- */

Log::Log(LogThreadPool *pool) : m_log_pool(pool)
{
}

Log::~Log()
{
    m_log_pool = nullptr;
}

/**
 * @brief 写普通日志信息
 * 
 * @param path : 日志文件路径
 * @param str : 日志内容
 */
void Log::i(const Path &path, const std::string &str)
{
    std::stringstream ss;
    ss << "[ Nomal Log ] : " << str << std::endl;
    LogBuffer* buf = new LogBuffer();
    buf->SetPath(path);
    buf->Buffer(ss.str());
    m_log_pool->Add(buf);
}

/**
 * @brief 警告信息日志
 * 
 * @param path : 日志文件路径
 * @param str : 日志内容
 */
void Log::r(const Path &path, const std::string &str)
{
    std::stringstream ss;
    ss << "[ Wraning Log ] : " << str << std::endl;
    LogBuffer* buf = new LogBuffer();
    buf->SetPath(path);
    buf->Buffer(ss.str());
    m_log_pool->Add(buf);
}

/**
 * @brief 错误日志
 * 
 * @param path : 日志文件路径
 * @param str : 日志内容
 */
void Log::e(const Path &path, const std::string &str)
{
    std::stringstream ss;
    ss << "[ Error Log ] : " << str << std::endl;
    LogBuffer* buf = new LogBuffer();
    buf->SetPath(path);
    buf->Buffer(ss.str());
    m_log_pool->Add(buf);
}