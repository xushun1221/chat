/*
 *  @Filename : MySQL.cc
 *  @Description : Implement of MySQL class
 *  @Datatime : 2022/11/28 13:37:11
 *  @Author : xushun
 */
#include "MySQL.hh"
#include <muduo/base/Logging.h>

// 数据库配置信息
static std::string server = "127.0.0.1";
static std::string user = "root";
static std::string password = "666666";
static std::string dbname = "chat";

MySQL::MySQL()
{
    conn_ = mysql_init(nullptr);
}

MySQL::~MySQL()
{
    if (conn_ != nullptr)
    {
        mysql_close(conn_);
    }
}

bool MySQL::connect()
{
    MYSQL *p = mysql_real_connect(conn_, server.c_str(), user.c_str(), password.c_str(), dbname.c_str(), 3306, nullptr, 0);
    if (p != nullptr)
    {
        mysql_query(conn_, "set names gbk");
    }
    else
    {
        LOG_INFO << __FILE__ << ":" << __LINE__ << ": connect fail!";
    }
    return p;
}

bool MySQL::update(std::string sql)
{
    if (mysql_query(conn_, sql.c_str()))
    {
        LOG_INFO << __FILE__ << ":" << __LINE__ << ":" << sql << " updata fail!";
        return false;
    }
    return true;
}

MYSQL_RES *MySQL::query(std::string sql)
{
    if (mysql_query(conn_, sql.c_str()))
    {
        LOG_INFO << __FILE__ << ":" << __LINE__ << ":" << sql << " query fail!";
        return nullptr;
    }
    return mysql_use_result(conn_);
}

MYSQL *MySQL::getConnection()
{
    return conn_;
}
