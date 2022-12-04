/*
 *  @Filename : MySQL.hh
 *  @Description : mysql interface
 *  @Datatime : 2022/11/28 13:19:27
 *  @Author : xushun
 */
#ifndef __MYSQL_HH_
#define __MYSQL_HH_

#include <mysql/mysql.h>
#include <string>

// 数据库操作类
class MySQL
{
public:
    // 初始化资源
    MySQL();
    // 释放资源
    ~MySQL();
    // 连接mysqlserver
    bool connect();
    // 更新操作
    bool update(std::string sql);
    // 查询操作
    MYSQL_RES *query(std::string sql);
    // 获取连接 可能会用到其他的方法
    MYSQL* getConnection();

private:
    MYSQL *conn_;
};

#endif // __MYSQL_HH_