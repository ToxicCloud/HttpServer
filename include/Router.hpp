/*
 * @Author: Toxic Cloud
 * @Date: 2021-04-02 22:50:01
 * @LastEditTime: 2021-04-03 14:52:25
 * @LastEditors: Please set LastEditors
 * @Description: Router类
 * @FilePath: /HttpServer/include/Utils/Router.hpp
 */
#pragma once

#include <functional>
#include "Task.hpp"
#include "Setting.hpp"

class Router
{
public:
    typedef std::function<void (Task& task)> CallBack;
private:
    std::unordered_map<std::string, CallBack> func_map;
    std::unordered_map<std::string, std::string> method_map;
    Setting* m_setting;
public:
    Router();
    virtual ~Router();
    // 设置Setting
    void InitSetting(Setting* setting);
    // 获取匹配的函数
    CallBack Patch(const std::string& path, const std::string& method);
    // 添加监听
    void On(const std::string& path, const std::string& method, CallBack call_back);
};
