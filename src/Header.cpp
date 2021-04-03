/*
 * @Author: Toxic Cloud
 * @Date: 2021-03-28 22:03:30
 * @LastEditTime: 2021-04-03 15:52:02
 * @LastEditors: Please set LastEditors
 * @Description: HTTP头部信息
 * @FilePath: /HttpServer/src/Header.cpp
 */
#include "Header.hpp"

Header::Header()
{
}

Header::~Header()
{
}

/**
 * @brief 获取HTTP头部值
 * 
 * @param key : 键
 * @return std::string 
 */
std::string Header::GetHead(const std::string &key) const
{
    std::string re;
    try
    {
        re = header_map.at(key);
    }
    catch (const std::out_of_range &out)
    {
        re = "";
    }
    return re;
}

/**
 * @brief 设置HTTP头部键值对
 * 
 * @param key : 键
 * @param value ：值
 * @return true 
 * @return false 
 */
bool Header::SetHead(const std::string &key, const std::string &value)
{
    header_map[key] = value;
    return true;
}

/**
 * @brief 清空头部信息
 * 
 */
void Header::Clear()
{
    header_map.clear();
}

/**
 * @brief 获取存储HTTP头部信息的map
 * 
 * @return std::unordered_map<std::string, std::string>& 
 */
std::unordered_map<std::string, std::string> & Header::Map()
{
    return header_map;
}