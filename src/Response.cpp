/*
 * @Author: Toxic Cloud
 * @Date: 2021-03-28 22:03:09
 * @LastEditTime: 2021-04-03 15:57:32
 * @LastEditors: Please set LastEditors
 * @Description: HTTP回应类
 * @FilePath: /HttpServer/src/Response.cpp
 */
#include "Response.hpp"

Response::Response(int sock)
{
    m_sock = sock;
    is_end = false;
    is_init = false;
}

Response::~Response()
{
    if (!is_end)
    {
        m_sock.Close();
    }
}

Header &Response::header()
{
    return m_header;
}

/**
 * @brief 将数字转换成字符串
 * 
 * @param code : 要转换的数字
 * @return std::string 
 */
std::string Response::NumToStr(int code)
{
    std::stringstream ss;
    ss << code;
    return ss.str();
}

/**
 * @brief 获取HTTP请求的路径
 * 
 * @return std::string& 
 */
std::string& Response::path()
{
    return m_url_path;
}

/**
 * @brief 通过Request类初始化Response中某些HTTP头部信息
 * 
 * @param req : Request类
 */
void Response::InitHeader(const Request &req)
{
    std::string buf;
    buf = req.GetHead("HttpVersin");
    if (buf != "")
    {
        m_http_version = buf;
    }
    else
    {
        m_http_version = "HTTP/1.1";
    }
    m_url_path = req.GetHead("Path");
    m_header.SetHead("Server", "Toxic/2.0");
    m_header.SetHead("Connection", "close");
    is_init = true;
}

/**
 * @brief 初始化文件后缀名对应Content-Type
 * 
 * @param path : 文件名
 */
void Response::InitContentType(const Path &path)
{
    std::string suffix = FileUtil::GetFileSuffix(path);
    if (suffix == "html" || suffix == "hml")
    {
        m_header.SetHead("Content-Type", "text/html;charset=UTF-8");
    }
    else if (suffix == "css")
    {
        m_header.SetHead("Content-Type", "text/css;charset=UTF-8");
    }
    else if (suffix == "js")
    {
        m_header.SetHead("Content-Type", "application/javascript;charset=UTF-8");
    }
    else if (suffix == "xml")
    {
        m_header.SetHead("Content-Type", "text/xml;charset=UTF-8");
    }
    else if (suffix == "gif")
    {
        m_header.SetHead("Content-Type", "image/gif");
    }
    else if (suffix == "jpeg" || suffix == "jpg" || suffix == "jpe")
    {
        m_header.SetHead("Content-Type", "image/jpeg");
    }
    else if (suffix == "png")
    {
        m_header.SetHead("Content-Type", "image/png");
    }
    else if (suffix == "json")
    {
        m_header.SetHead("Content-Type", "application/json");
    }
    else if (suffix == "pdf")
    {
        m_header.SetHead("Content-Type", "application/pdf");
    }
    else if (suffix == "word")
    {
        m_header.SetHead("Content-Type", "application/msword");
    }
    else if (suffix == "ico")
    {
        m_header.SetHead("Content-Type", "image/x-icon");
    }
    else if (suffix == "mp3")
    {
        m_header.SetHead("Content-Type", "video/mp3");
    }
    else if (suffix == "mp4")
    {
        m_header.SetHead("Content-Type", "video/mpeg4");
    }
    else if (suffix == "mpeg")
    {
        m_header.SetHead("Content-Type", "video/mpg");
    }
    else if (suffix == "txt")
    {
        m_header.SetHead("Content-Type", "text/plain;charset=UTF-8");
    }
    else
    {
        m_header.SetHead("Content-Type", "application/octet-stream");
    }
}

/**
 * @brief 发送消息前最后初始化
 * 
 */
void Response::Init()
{
    if (!is_init)
    {
        m_header.SetHead("Server", "Toxic/2.0");
        m_header.SetHead("Connection", "close");
        m_http_version = "HTTP/1.1";
    }
    m_head_content = m_http_version + " " + m_status + " OK\r\n";
    m_head_content += ("Content-Length: " + this->NumToStr(m_content.length()) + "\r\n");
    for (auto &item : m_header.Map())
    {
        m_head_content += (item.first + ": " + item.second + "\r\n");
    }
    m_head_content += "\r\n";
}

/**
 * @brief 内部发送函数
 * 
 */
void Response::Send()
{
    this->Init();
    ssize_t sen_len = 0;
    ssize_t len = 0;
    for (;;)
    {
        if (m_sock.IsClosed())
        {
            break;
        }
        sen_len = send(m_sock, m_head_content.c_str(), m_head_content.length() - len, MSG_NOSIGNAL);
        if (sen_len <= 0)
        {
            break;
        }
        len += sen_len;
        if (len >= static_cast<ssize_t>(m_head_content.length()))
        {
            break;
        }
    }
    len = 0;
    for (;;)
    {
        if (m_sock.IsClosed())
        {
            break;
        }
        sen_len = send(m_sock, m_content.c_str(), m_content.length() - len, MSG_NOSIGNAL);
        if (sen_len <= 0)
        {
            break;
        }
        len += sen_len;
        if (len >= static_cast<ssize_t>(m_content.length()))
        {
            break;
        }
    }
    m_sock.ClearBuffer();
    m_sock.Close();
    is_end = true;
}

/**
 * @brief 发送文字
 * 
 * @param code : 对应状态码
 * @param str ：要发送的字符内容
 */
void Response::SenText(int code, const std::string &str)
{
    if (is_end){
        // 套接字已关闭
        return;
    }
    m_status = this->NumToStr(code);
    m_header.SetHead("Content-Type", "text/plain;charset=UTF-8");
    m_content = str;
    Send();
}

/**
 * @brief 发送文件
 * 
 * @param path : 文件路径
 */
void Response::SendFile(const Path &path)
{
    if (is_end){
        // 套接字已关闭
        return;
    }
    this->InitContentType(path);
    if (!path.Exist()){
        // 文件不存在
        this->NotFound();
        return;
    }
    m_status = this->NumToStr(200);
    m_content = FileUtil::ReadFromFile(path);
    if (m_content.length() != FileUtil::FileSize(path)){
        // 读取错误
        this->Error("Sorry, We have some problems when we read the file !!!\n");
        return;
    }
    this->Send();
}

/**
 * @brief 不发送任何内容
 * 
 * @param code 
 */
void Response::End(int code)
{
    m_content = "";
    m_status = this->NumToStr(code);
    this->Send();
}

/**
 * @brief 发送错误信息
 * 
 * @param str : 错误信息
 */
void Response::Error(const std::string& str)
{
    std::string content = str;
    if (content == ""){
        content = "Sorry , You can't " + m_header.GetHead("Method") + "the path " + m_url_path + "\n";
    }
    this->SenText(400, content);
}

/**
 * @brief 发送文件未找到信息
 * 
 */
void Response::NotFound()
{
    std::string content = "Sorry, We can't find the path : " + m_url_path + " !!!\n";
    this->SenText(404, content);
}