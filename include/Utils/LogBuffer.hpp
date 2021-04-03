/*
 * @Author: Toxci Cloud
 * @Date: 2021-03-28 14:04:36
 * @LastEditTime: 2021-04-03 15:32:59
 * @LastEditors: Please set LastEditors
 * @Description: 日志记录
 * @FilePath: /HttpServer/include/Utils/LogBuffer.hpp
 */
#pragma once

#include "FileUtil.hpp"
#include "LogThreadPool.hpp"
#include <functional>
#include <unordered_map>

// 日志缓存
class LogBuffer
{
public:
    enum Log_Status{
        LOG_WAITING = 0x01,
        LOG_DOWN = 0x02,
        LOG_ERROR = 0x00
    };
private:
    Path m_path;
    std::stringstream m_buffer;
    // 日志状态
    Log_Status m_status;
public:
    LogBuffer();
    virtual ~LogBuffer();
    // 设置日志路径
    void SetPath(const Path& path);
    void Buffer(const std::string& str);
    Log_Status Status() const;
    void run();
};

class Log
{
private:
    LogThreadPool* m_log_pool;
public:
    Log(LogThreadPool* pool);
    ~Log();
    // 普通日志
    void i(const Path& path, const std::string& str);
    // 警告日志
    void r(const Path& path, const std::string& str);
    // 错误日志
    void e(const Path& path, const std::string& str);
};

