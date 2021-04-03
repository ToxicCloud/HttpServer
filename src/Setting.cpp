/*
 * @Author: Toxic Cloud
 * @Date: 2021-04-03 13:18:25
 * @LastEditTime: 2021-04-03 14:49:56
 * @LastEditors: Please set LastEditors
 * @Description: Setting类
 * @FilePath: /HttpServer/src/Setting.cpp
 */

#include "Setting.hpp"

Setting::Setting()
{
    
}

Setting::~Setting()
{

}

/**
 * @brief 获取属性
 * 
 * @param key : 关键字
 * @return std::string 
 */
std::string Setting::Set(const std::string& key)
{
    std::string result;
    try{
        result = m_set_map.at(key);
    }catch(const std::out_of_range& r){
        result = "";
    }
    return result;
}

/**
 * @brief 设置属性
 * 
 * @param key : 关键字
 * @param value : 值
 * @return Setting& 
 */
Setting& Setting::Set(const std::string& key, const std::string& value)
{
    m_set_map[key] = value;
    return *this;
}