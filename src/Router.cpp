/*
 * @Author: Toxic Cloud
 * @Date: 2021-04-02 22:50:08
 * @LastEditTime: 2021-04-03 15:59:17
 * @LastEditors: Please set LastEditors
 * @Description: Router类
 * @FilePath: /HttpServer/src/Router.cpp
 */
#include "Router.hpp"

Router::Router()
{
}

Router::~Router()
{
}

/**
 * @brief 添加监听
 * 
 * @param path ：监听路径
 * @param method ：监听方法
 * @param call_back ：对应监听回调
 */
void Router::On(const std::string &path, const std::string &method, Router::CallBack call_back)
{
    func_map[path] = call_back;
    method_map[path] = method;
}

/**
 * @brief 获取匹配的函数
 * 
 * @param path : 请求路径
 * @param method ：请求方法
 * @return Router::CallBack 
 */
Router::CallBack Router::Patch(const std::string &path, const std::string &method)
{
    CallBack back = nullptr;
    std::string m_method;
    try
    {
        back = func_map.at(path);
        m_method = method_map.at(path);
    }
    catch (const std::out_of_range &r)
    {
        back = nullptr;
    }
    if (back == nullptr || m_method != method)
    {
        back = [&](Task &task) -> void {
            if (task.response->path() == "/"){
                task.response->path() = "/index.html";
            }
            std::string web_home = FileUtil::GetExecPath() + "/WebHome";
            task.response->SendFile(web_home + task.response->path());
        };
    }
    return back;
}

/**
 * @brief 设置Setting
 * 
 * @param setting : Setting类
 */
void Router::InitSetting(Setting *setting)
{
    m_setting = setting;
}