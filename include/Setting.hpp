/*
 * @Author: Toxic Cloud
 * @Date: 2021-04-03 13:18:17
 * @LastEditTime: 2021-04-03 13:31:33
 * @LastEditors: Please set LastEditors
 * @Description: Setting类
 * @FilePath: /HttpServer/include/Setting.hpp
 */
#include "FileUtil.hpp"
#include <unordered_map>

class Setting
{
private:
    std::unordered_map<std::string, std::string> m_set_map;
public:
    Setting();
    virtual ~Setting();

    // 设置与获取属性
    std::string Set(const std::string& key);
    Setting& Set(const std::string& key, const std::string& value);
};