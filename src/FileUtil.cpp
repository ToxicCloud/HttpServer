/*
 * @Author: Toxic Cloud
 * @Date: 2021-03-28 11:58:57
 * @LastEditTime: 2021-04-03 15:44:20
 * @LastEditors: Please set LastEditors
 * @Description: 文件操作类
 * @FilePath: /HttpServer/src/FileUtil.cpp
 */
#include "FileUtil.hpp"

Path::Path(const std::string &path) : m_path(path)
{
}

Path::Path(const char *path) : m_path(path)
{
}

Path::~Path()
{
}

/**
 * @brief 设置路径
 * 
 * @param path : 路径
 * @return void
 */
void Path::Set(const std::string &path)
{
    m_path = path;
}

/**
 * @brief 将路径转换成字符串格式
 * 
 * @return std::string 
 */
std::string Path::ToString() const
{
    return m_path;
}

/**
 * @brief 将路径转换成C语言字符串格式
 * 
 * @return const char* 
 */
const char *Path::C_Str() const
{
    return m_path.c_str();
}

/**
 * @brief 判断路径是否存在
 * 
 * @return true 
 * @return false 
 */
bool Path::Exist() const
{
    struct stat s;
    int code = stat(m_path.c_str(), &s);
    if (code < 0)
    {
        return false;
    }
    return true;
}

/**
 * @brief 返回路径类型
 * 
 * @return Path::Path_Type 
 */
Path::Path_Type Path::PathType() const
{
    struct stat s;
    int code = stat(m_path.c_str(), &s);
    if (code < 0)
    {
        return PATH_ERR;
    }
    if (S_ISREG(s.st_mode))
    {
        return PATH_FILE;
    }
    else if (S_ISDIR(s.st_mode))
    {
        return PATH_DIR;
    }
    else if (S_ISLNK(s.st_mode))
    {
        return PATH_LINK;
    }
    else if (S_ISSOCK(s.st_mode))
    {
        return PATH_SOCKET;
    }
    return PATH_OTHER;
}

// = 操作符重载
Path &Path::operator=(const std::string &path)
{
    m_path = path;
    return *this;
}

Path &Path::operator=(const char *path)
{
    m_path = path;
    return *this;
}

// 显式或者隐式转换成字符串
Path::operator std::string()
{
    return m_path;
}

/**
 * @brief 创建路径
 * 
 * @return true 
 * @return false 
 */
bool Path::Create() const
{
    if (this->Exist()){
        return true;
    }
    std::string temp(m_path);
    std::string dir = "";
    size_t index = 0;
    size_t count = 0;
    long total = -1;
    while (true)
    {
        index = temp.find_first_of('/');
        if (index == std::string::npos){
            break;
        }
        for (size_t i = 0; i <= index; ++i){
            temp.erase(temp.begin());
        }
        ++count;
        total += index;
        dir = m_path.substr(0, total + count);
        Path t_path(dir);
        if (dir == "" || t_path.Exist()){
            continue;
        }
        int code = mkdir(dir.c_str(), S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO);
        if (code == -1){
            return false;
        }
    }
    std::ofstream of(m_path);
    if (!of.is_open()){
        return false;
    }
    of.close();
    return true;
}

/**
 * @brief 销毁路径
 * 
 * @return true 
 * @return false 
 */
bool Path::Destroy() const
{
    if (!this->Exist()){
        return true;
    }
    return true;
}


/* ----------------------------------------------------------------------------------------------------------------------- */

/**
 * @brief 获取文件大小
 * 
 * @param path : 文件路径
 * @return size_t 
 */
size_t FileUtil::FileSize(const Path &path)
{
    struct stat s;
    int code = stat(path.C_Str(), &s);
    if (code < 0)
    {
        return 0;
    }
    return static_cast<size_t>(s.st_size);
}

/**
 * @brief 读取文件
 * 
 * @param path : 路径
 * @return std::string 
 */
std::string FileUtil::ReadFromFile(const Path &path)
{
    std::ifstream in(path.C_Str());
    if (!in.is_open())
    {
        return "";
    }
    std::ostringstream ss;
    ss << in.rdbuf();
    in.close();
    return ss.str();
}

/**
 * @brief 写入文件
 * 
 * @param path : 路径
 * @return true 
 * @return false 
 */
bool FileUtil::WriteToFile(const Path &path, const std::string &str)
{
    std::ofstream of(path.C_Str());
    if (!of.is_open())
    {
        return false;
    }
    std::stringstream ss;
    ss << str;
    of << ss.rdbuf();
    of.close();
    return true;
}

/**
 * @brief 获取文件后缀
 * 
 * @param path : 文件路径
 * @return std::string 
 */
std::string FileUtil::GetFileSuffix(const Path &path)
{
    if (!path.Exist())
    {
        return "";
    }
    size_t code = path.ToString().find_last_of('.');
    if (code == std::string::npos)
    {
        return "";
    }
    return path.ToString().substr(code + 1);
}

/**
 * @brief 获取当前执行进程的绝对路径
 * 
 * @return std::string 
 */
std::string FileUtil::GetExecPath()
{
    char buffer[256];
    char* code = getcwd(buffer, sizeof(buffer));
    if (!code){
        return "";
    }
    return buffer;
}
