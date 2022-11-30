/*
 *  @Filename : FriendModel.cc
 *  @Description : Implementation of FriendModel
 *  @Datatime : 2022/11/30 14:58:22
 *  @Author : xushun
 */

#include "FriendModel.hh"
#include "db.hh"

bool FriendModel::insert(uint32_t userid, uint32_t friendid)
{
    // 组装sql
    char sql[1024] = {0};
    sprintf(sql, "INSERT INTO friend(userid,friendid) VALUES('%ld','%ld')", userid, friendid);
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

std::vector<User> FriendModel::queryById(uint32_t userid)
{
    // 用于返回好友列表 如果没有返回空vector
    std::vector<User> vec;
    // 组装sql user和friend表的联合查询
    char sql[1024] = {0};
    sprintf(sql, "SELECT u.id,u.name,u.state FROM user u INNER JOIN friend f ON f.friendid=u.id WHERE f.userid=%ld", userid);
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
                User user;
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setState(row[2]);
                vec.push_back(user);
            }
            mysql_free_result(res); // 别忘了释放资源
        }
    }
    return vec;
}