/*
 * @Author: Toxic Cloud
 * @Date: 2021-03-21 11:54:35
 * @LastEditTime: 2021-03-31 12:15:18
 * @LastEditors: Please set LastEditors
 * @Description: HTTP请求解析和保存
 * @FilePath: /CppRestful/include/Request.hpp
 */

#ifndef REQUEST_HPP_
#define REQUEST_HPP_

#include <vector>
#include <memory>
#include <cstring>
#include <iostream>
#include "Header.hpp"
#include "TException.hpp"
#include "Socket.hpp"

class Request
{
public:
    enum ParseCode
    {
        PARSE_OK = 0x00,
        PARSE_ERROE = 0x01,
        PARSE_NOTFULL = 0x02,
        PARSE_INERROR = 0x04
    };
private:
    Header m_header;
    // 请求状态
    ParseCode parse_code;
    // 存储每一行请求数据(便于解析)
    std::vector<std::string> m_heads;
    // Socket
    Socket m_socket;
private:
    // 设置头部信息
    bool SetHead(const std::string& key, const std::string& value);
    // 内部请求解析初始化（将请求解析到数据数组）
    ParseCode ParseInit(const std::string request_buf);
    // 内部解析主函数
    ParseCode Parse();
    // 解析第一行
    ParseCode ParseFirstLine(const std::string& first_line);
    // 解析其他行
    ParseCode ParseOtherLines(const std::string& other_line);
public:
    Request(int sock = -1);
    virtual ~Request();
    // 设置头部
    bool SetHeader(const Header& header);
    // 获取头部信息
    std::string GetHead(const std::string& key) const;

    // 从Socket中解析
    ParseCode ParseFromSocket(Socket& socket);
    ParseCode ParseFromSocket();
    // 从String中解析
    ParseCode ParseFromString(const std::string& str);
};

#endif