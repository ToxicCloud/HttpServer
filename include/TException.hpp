/*
 * @Author: Toxic Cloud
 * @Date: 2021-03-31 11:16:08
 * @LastEditTime: 2021-04-03 16:31:26
 * @LastEditors: Please set LastEditors
 * @Description: None
 * @FilePath: /HttpServer/include/TException.hpp
 */
#pragma once

#include <string>

class TException
{
private:
    /* data */
    std::string m_err;
public:
    TException(const std::string& str);
    ~TException();
    std::string& what();
};