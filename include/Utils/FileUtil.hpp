/*
 * @Author: Toxic Cloud
 * @Date: 2021-03-28 11:44:35
 * @LastEditTime: 2021-04-03 15:44:41
 * @LastEditors: Please set LastEditors
 * @Description: 文件管理类
 * @FilePath: /HttpServer/include/Utils/FileUtil.hpp
 */
#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <memory>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>

// 路径类
class Path
{
private:
    std::string m_path;
public:
    // 文件类型
    enum Path_Type{
        PATH_FILE = 0x01,               // 文件
        PATH_DIR = 0x02,                // 文件夹
        PATH_LINK = 0x03,               // 链接
        PATH_SOCKET = 0x04,             // Socket
        PATH_OTHER = 0x05,              // 其他类型
        PATH_ERR = 0x06                 // 发生错误
    };
public:
    Path(const std::string& path = "");
    Path(const char* path = "");
    virtual ~Path();
    // 设置路径
    void Set(const std::string& path);
    // 转换成字符串格式
    std::string ToString() const;
    // 转换成c语言格式的str
    const char* C_Str() const;
    // 判断路径是否存在
    bool Exist() const;
    // 返回文件类型
    Path_Type PathType() const;
    // 创建路径
    bool Create() const;
    // 销毁路径
    bool Destroy() const;

    Path& operator = (const std::string& path);
    Path& operator = (const char* path);
    operator std::string();

};

namespace FileUtil
{
    // 获取文件大小
    size_t FileSize(const Path& path);
    // 读取文件
    std::string ReadFromFile(const Path& path);
    // 写入文件
    bool WriteToFile(const Path& path, const std::string& str);
    // 获取文件后缀
    std::string GetFileSuffix(const Path& path);
    std::string GetExecPath();
}
