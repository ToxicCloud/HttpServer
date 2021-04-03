/*
 * @Author: Toxic Cloud
 * @Date: 2021-03-22 20:11:02
 * @LastEditTime: 2021-04-03 16:32:38
 * @LastEditors: Please set LastEditors
 * @Description: HTTP请求解析类
 * @FilePath: /CppRestful/src/Request.cpp
 */
#include "Request.hpp"


Request::Request(int sock)
{
    m_socket = sock;
}

Request::~Request()
{
}

/**
 * @brief 设置头部信息(私有函数)
 * 
 * @param key ：键
 * @param value ：值
 * @return true 
 * @return false 
 */
bool Request::SetHead(const std::string& key, const std::string& value)
{
    return m_header.SetHead(key, value);
}

/**
 * @brief 获取头部信息
 * 
 * @param key : 键
 * @return std::string 
 */
std::string Request::GetHead(const std::string& key) const
{
    return m_header.GetHead(key);
}

bool Request::SetHeader(const Header& header)
{
    m_header = header;
    return true;
}

/**
 * @brief 解析初始化（将请求按行存放到行数组）
 * 
 * @param request_buf : 解析字符串
 * @return Request::ParseCode 
 */
Request::ParseCode Request::ParseInit(const std::string request_buf)
{
    // 请求长度
    size_t buf_len = request_buf.length();
    // 临时字符
    char temp_char;
    // 是否出现 '/r'
    bool is_r = false;
    // 行开始下标
    size_t line_start = 0;
    // 存储字符串副本
    std::unique_ptr<char> buffer(new char[buf_len + 1]);
    std::strcpy(buffer.get(), request_buf.c_str());

    for (size_t index = 0; index < buf_len; ++index){
        temp_char = buffer.get()[index];
        // 遇到 '\r'
        if (temp_char == '\r'){
            is_r = true;
            // 如果这是最后一个字符，说明请求不完整
            if (index + 1 >= buf_len){
                return Request::PARSE_NOTFULL;
            }
            // 如果下一个字符存在并且为 '\n'，说明这一行结束了
            else if (index < (buf_len - 1) && buffer.get()[index + 1] == '\n'){
                // 将\r 与 \n 赋值为字符串结尾符号
                buffer.get()[index++] = '\0';
                buffer.get()[index] = '\0';
                // 添加到行数组
                char* line = buffer.get() + line_start;
                m_heads.push_back(line);
                // 调整行开始下标 
                line_start = index + 1;
            }
            else{
                TException exp("请求解析错误 -- 请求解析格式错误");
                throw exp;
                return Request::PARSE_ERROE;
            }
        }
    }
    if (!is_r){
        return Request::PARSE_NOTFULL;
    }
    // 如果出现过\r但是行数组最后不是\0， 说明请求解析不完整
    else if (is_r && m_heads[m_heads.size() - 1][0] != '\0'){
        return Request::PARSE_NOTFULL;
    }
    else if (is_r && m_heads[m_heads.size() - 1][0] == '\0'){
        return Request::PARSE_OK;
    }
    TException exp("解析错误 -- 未知错误");
    throw exp;
    return Request::PARSE_INERROR;
}

/**
 * @brief 解析请求第一行
 * 
 * @param first_line 
 * @return Request::ParseCode 
 */
Request::ParseCode Request::ParseFirstLine(const std::string& first_line)
{
    std::unique_ptr<char> buffer(new char[first_line.length() + 1]);
    std::strcpy(buffer.get(), first_line.c_str());
    char* path = std::strpbrk(buffer.get(), " \t");
    if (!path){
        TException exp("解析错误 -- 格式错误");
        throw exp;
        return Request::PARSE_ERROE;
    }
    *path++ = '\0';
    // 获取请求方法
    char* method = buffer.get();
    m_header.SetHead("Method", method);
    method = nullptr;
    path += std::strspn(path, " \t");
    
    // 获取HTTP版本号
    char* version = std::strpbrk(path, " \t");
    if (!version){
        TException exp("解析错误 -- 格式错误");
        throw exp;
        return Request::PARSE_ERROE;
    }
    *version++ = '\0';
    version += std::strspn(version, " \t");
    if (!strcasecmp(version, "HTTP/1.1")){
        m_header.SetHead("HttpVersion", version);
    }
    version = nullptr;
    if (!path || path[0] != '/'){
        TException exp("解析错误 -- 格式错误");
        throw exp;
        return Request::PARSE_ERROE;
    }
    m_header.SetHead("Path", path);
    return Request::PARSE_OK;
}

/**
 * @brief 解析其他行
 * 
 * @param other_line : 其他行 
 * @return Request::ParseCode 
 */
Request::ParseCode Request::ParseOtherLines(const std::string& other_line)
{
    if (other_line[0] == '\0'){
        return Request::PARSE_OK;
    }

    // other_lines的副本
    std::unique_ptr<char> buffer(new char[other_line.length() + 1]);
    strcpy(buffer.get(), other_line.c_str());

    char *value = strpbrk(buffer.get(), " \t");
    if (!value)
    {
        // 抛出错误
        TException exp("请求解析错误 -- 请求格式错误!");
        throw exp;
        return Request::PARSE_ERROE;
    }
    *value++ = '\0';

    // 检查属性名和值
    char *key = buffer.get();
    value += strspn(value, " \t");
    if (!key || !strpbrk(key, ":") || !value)
    {
        // 抛出错误
        TException exp("请求解析错误 -- 格式错误!");
        throw exp;
        return Request::PARSE_ERROE;
    }
    key[strlen(key) - 1] = '\0';
    m_header.SetHead(key, value);
    return Request::PARSE_OK;
}

/**
 * @brief 解析主函数
 * 
 * @return Request::ParseCode 
 */
Request::ParseCode Request::Parse()
{
    ParseCode code;
    for (size_t index = 0; index < m_heads.size(); ++index){
        // 解析第一行
        if (index == 0){
            code = ParseFirstLine(m_heads[index]);
            if (code == Request::PARSE_ERROE){
                m_header.Clear();
                m_heads.clear();
                return Request::PARSE_ERROE;
            }
        }else{
            code = ParseOtherLines(m_heads[index]);
            if (code == Request::PARSE_ERROE){
                m_header.Clear();
                m_heads.clear();
                return Request::PARSE_ERROE;
            }
        }
    }
    return Request::PARSE_OK;
}

/**
 * @brief 从Socket解析头部信息
 * 
 * @param socket : 套接字
 * @return true 
 * @return false 
 */
Request::ParseCode Request::ParseFromSocket(Socket& sock)
{
    // 接受请求的缓存
    std::unique_ptr<char> rev_buffer(new char[2048]);
    // 总缓存
    std::string buffer = "";
    // 接受字符个数
    ssize_t rev_len = 0;
    // 解析状态
    ParseCode re_code = PARSE_OK;
    while (1){
        std::memset(rev_buffer.get(), 0, sizeof(*rev_buffer.get()));
        if (sock.IsClosed()){
            break;
        }
        rev_len = recv(sock.Get(), rev_buffer.get(), 2048, MSG_NOSIGNAL);
        if (rev_len < 0){
            re_code = PARSE_ERROE;
            break;
        }
        if (rev_len == 0){
            re_code = PARSE_OK;
            break;
        }
        buffer += rev_buffer.get();
        // 初始化请求，判断请求是否完整
        ParseCode code = ParseInit(buffer);
        if (code == PARSE_OK){
            // 请求是完整的， 进行解析
            re_code = Parse();
            break;
        }
        else if (code == PARSE_NOTFULL){
            // 请求不完整，清除之前的解析
            m_heads.clear();
            m_header.Clear();
            continue;
        }
        else if (code == PARSE_ERROE){
            // 请求解析错误
            re_code = PARSE_ERROE;
            m_heads.clear();
            m_header.Clear();
            break;
        }
        re_code = PARSE_INERROR;
    }
    return re_code;
}

/**
 * @brief 从套接字中解析
 * 
 * @return Request::ParseCode 
 */
Request::ParseCode Request::ParseFromSocket()
{
    if (m_socket.Get() != -1){
        return ParseFromSocket(m_socket);
    }
    return PARSE_ERROE;
}

/**
 * @brief 从字符串中解析HTTP请求
 * 
 * @param str : 要解析的字符串
 * @return Request::ParseCode 
 */
Request::ParseCode Request::ParseFromString(const std::string& str)
{
    ParseCode code = ParseInit(str);
    if (code == PARSE_ERROE){
        m_heads.clear();
        return PARSE_ERROE;
    }
    else if (code == PARSE_NOTFULL){
        m_heads.clear();
        TException exp("请求解析错误 -- 不完整的请求");
        throw exp;
        return PARSE_NOTFULL;
    }
    else if (code == PARSE_OK){
        return Parse();
    }
    return PARSE_INERROR;
}
