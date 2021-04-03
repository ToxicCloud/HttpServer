/*
 * @Author: Toxic Cloud
 * @Date: 2021-03-31 11:17:51
 * @LastEditTime: 2021-04-03 16:33:39
 * @LastEditors: Please set LastEditors
 * @Description: None
 * @FilePath: /HttpServer/src/TException.cpp
 */
#include "TException.hpp"

TException::TException(const std::string& str)
{
    m_err = str;
}

TException::~TException()
{

}

std::string& TException::what()
{
    return m_err;
}