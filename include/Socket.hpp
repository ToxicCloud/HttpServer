/*
 * @Author: Toxic Cloud
 * @Date: 2021-03-28 22:16:00
 * @LastEditTime: 2021-03-31 10:21:14
 * @LastEditors: Please set LastEditors
 * @Description: Socket封装
 * @FilePath: /HttpServer/include/Socket.hpp
 */
#pragma once

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <linux/ip.h>
#include <netinet/tcp.h>
#include <sys/fcntl.h>
#include <cstring>

class Socket
{
private:
    /* data */
    int m_sock;
    tcp_info m_info;
public:
    Socket();
    Socket(int sock);
    virtual ~Socket();

    // 关闭套接字
    void Close();
    // 关闭套接字
    void ShutDown(int what);
    // 判断套接字是否关闭
    bool IsClosed();
    // 获得套接字
    int Get() const;
    // 设置套接字非阻塞
    void SetNoBlocking();
    // 清空缓冲区数据
    void ClearBuffer();
    Socket& operator = (int sock);
    operator int();
};

