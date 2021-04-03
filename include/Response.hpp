/*
 * @Author: Toxic Cloud
 * @Date: 2021-03-28 22:02:53
 * @LastEditTime: 2021-04-03 15:20:38
 * @LastEditors: Please set LastEditors
 * @Description: HTTP回应类
 * @FilePath: /HttpServer/include/Response.hpp
 */
#pragma once

#include "Socket.hpp"
#include "Header.hpp"
#include "Request.hpp"
#include "FileUtil.hpp"
#include "LogBuffer.hpp"

class Response
{
private:
    // 回应套接字
    Socket m_sock;
    // 消息是否已经发送过
    bool is_end;
    // 是否初始化
    bool is_init;
    // 头部信息
    Header m_header;
    // Http版本
    std::string m_http_version;
    // 对应状态码
    std::string m_status;
    // 路径
    std::string m_url_path;
    //  消息头
    std::string m_head_content;
    // 消息体
    std::string m_content;
public:
    Response(int sock = -1);
    virtual ~Response();

    // 获取头部信息
    Header& header();
    // 初始化头部信息
    void InitHeader(const Request& req);
    // 发送文字
    void SenText(int code, const std::string& str);
    // 发送文件
    void SendFile(const Path& path);
    void End(int code);
    // 发送错误信息
    void Error(const std::string& err = "");
    // 发送文件未找到
    void NotFound();
    // 获取文件路径
    std::string& path();
private:
    // 内部发送函数
    void Send();
    // 初始化文件后缀名对应Content-Type
    void InitContentType(const Path& path);
    // 发送消息前最后初始化
    void Init();
    std::string NumToStr(int code);
};