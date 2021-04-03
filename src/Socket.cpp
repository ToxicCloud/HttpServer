/*
 * @Author: Toxic Cloud
 * @Date: 2021-03-28 22:16:07
 * @LastEditTime: 2021-04-03 16:06:23
 * @LastEditors: Please set LastEditors
 * @Description: Socket封装
 * @FilePath: /HttpServer/src/Socket.cpp
 */
#include "Socket.hpp"

Socket::Socket()
{
    m_sock = -1;
}

Socket::Socket(int sock)
{
    m_sock = sock;
}

Socket::~Socket()
{
}

/**
 * @brief 关闭套接字
 * 
 */
void Socket::Close()
{
    if (m_sock != -1)
    {
        close(m_sock);
        m_sock = -1;
    }
}

/**
 * @brief Shudown的方式关闭套接字
 * 
 * @param what 
 */
void Socket::ShutDown(int what)
{
    if (m_sock != -1)
    {
        shutdown(m_sock, what);
        m_sock = -1;
    }
}

/**
 * @brief 判断套接字是否断开连接
 * 
 * @return true 
 * @return false 
 */
bool Socket::IsClosed()
{
    std::memset(&m_info, 0, sizeof(m_info));
    socklen_t info_len = sizeof(m_info);
    getsockopt(m_sock, IPPROTO_TCP, TCP_INFO, &m_info, &info_len);
    if (m_info.tcpi_state != 1)
    {
        return true;
    }
    return false;
}

/**
 * @brief 获取套接字
 * 
 * @return int 
 */
int Socket::Get() const
{
    return m_sock;
}

/**
 * @brief 设置套接字非阻塞
 * 
 */
void Socket::SetNoBlocking()
{
    int old_flag = fcntl(m_sock, F_GETFL);
    int new_flag = old_flag | O_NONBLOCK;
    fcntl(m_sock, F_SETFL, new_flag);
}

/**
 * @brief 清空套接字缓冲区数据
 * 
 */
void Socket::ClearBuffer()
{
    // 设置select立即返回
    timeval time_out;
    time_out.tv_sec = 0;
    time_out.tv_usec = 0;
    
    // 设置select对sock_conn的读取感兴趣
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(m_sock, &read_fds);
 
    int res = -1;
    char recv_data[2];
    memset(recv_data, 0, sizeof(recv_data));
    while(true){
        res = select(FD_SETSIZE, &read_fds, nullptr, nullptr, &time_out);
        if (res == 0) break;  //数据读取完毕，缓存区清空成功
        recv(m_sock, recv_data, 1, 0);  //触发数据读取
    }
}

Socket &Socket::operator=(int sock)
{
    m_sock = sock;
    return *this;
}

Socket::operator int()
{
    return m_sock;
}