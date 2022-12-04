/*
 *  @Filename : OfflineMessageModel.cc
 *  @Description : Implementation of OfflineMessageModel
 *  @Datatime : 2022/11/30 10:28:08
 *  @Author : xushun
 */

#include "OfflineMessageModel.hh"
#include "MySQL.hh"

bool OfflineMessageModel::insert(uint32_t userid, std::string msg)
{
    // 组装sql
    char sql[1024] = {0};
    sprintf(sql, "INSERT INTO offlinemessage(userid,message) VALUES('%ld','%s')", userid, msg.c_str());
    // 连接mysqlserver并插入记录
    MySQL mysql;
    if (mysql.connect())
    {
        if (mysql.update(sql))
        {
            return true;
        }
    }
    return false;
}

bool OfflineMessageModel::remove(uint32_t userid)
{
    // 组装sql
    char sql[1024] = {0};
    sprintf(sql, "DELETE FROM offlinemessage WHERE userid=%ld", userid);
    // 连接mysqlserver并删除记录
    MySQL mysql;
    if (mysql.connect())
    {
        if (mysql.update(sql))
        {
            return true;
        }
    }
    return false;
}

std::vector<std::string> OfflineMessageModel::queryById(uint32_t userid)
{
    // 用于返回所有的离线消息 如果没有返回空vector
    std::vector<std::string> vec;
    // 组装sql
    char sql[1024] = {0};
    sprintf(sql, "SELECT message FROM offlinemessage WHERE userid=%ld", userid);
    // 连接mysqlserver并查询
    MySQL mysql;
    if (mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);
        if (res)
        {
            MYSQL_ROW row;
            while (row = mysql_fetch_row(res))
            {
                vec.push_back(row[0]);
            }
            mysql_free_result(res); // 别忘了释放资源
        }
    }
    return vec;
}