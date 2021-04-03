/*
 * @Author: Toxic Cloud
 * @Date: 2021-03-28 22:03:24
 * @LastEditTime: 2021-04-02 21:24:07
 * @LastEditors: Please set LastEditors
 * @Description: HTTP头部信息
 * @FilePath: /HttpServer/include/Utils/Header.hpp
 */
#pragma once
#include <string>
#include <unordered_map>

class Header
{
private:
    /* data */
    // 头部信息键值对
    std::unordered_map<std::string, std::string> header_map;
public:
    Header();
    virtual ~Header();

    // 获取对应键值
    std::string GetHead(const std::string& key) const;
    // 设置对应键值对
    bool SetHead(const std::string& key, const std::string& value);
    // 删除全部键值对
    void Clear();
    // 获取键值对
    std::unordered_map<std::string, std::string>& Map();
};