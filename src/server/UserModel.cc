/*
 *  @Filename : UserModel.cc
 *  @Description : Implementation of UserModel
 *  @Datatime : 2022/11/28 14:38:47
 *  @Author : xushun
 */
#include "UserModel.hh"
#include "MySQL.hh"
#include <iostream>

bool UserModel::insert(User &user)
{
    // 组装sql
    char sql[1024] = {0};
    sprintf(sql, "INSERT INTO user(name, password, state) VALUES('%s', '%s', '%s')",
            user.getName().c_str(), user.getPassword().c_str(), user.getState().c_str());
    // 连接mysqlserver并插入记录
    MySQL mysql;
    if (mysql.connect())
    {
        if (mysql.update(sql))
        {
            // 插入新记录并返回记录的主键ID
            user.setId(mysql_insert_id(mysql.getConnection()));
            return true;
        }
    }
    return false;
}

User UserModel::queryById(uint32_t id)
{
    // 组装sql
    char sql[1024] = {0};
    sprintf(sql, "SELECT * FROM user WHERE id = %ld", id);
    // 连接mysqlserver并查询
    MySQL mysql;
    if (mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);
        if (res)
        {
            MYSQL_ROW row = mysql_fetch_row(res);
            if (row)
            {
                User user;
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setPassword(row[2]);
                user.setState(row[3]);
                mysql_free_result(res); // 别忘了释放资源
                return user;
            }
        }
    }
    // 如果连接失败或者没有找到相应的id 返回一个默认的User对象 id为0
    return User();
}

bool UserModel::updateState(User &user)
{
    // 组装sql
    char sql[1024] = {0};
    sprintf(sql, "UPDATE user SET state = '%s' WHERE id = %ld", user.getState().c_str(), user.getId());
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

bool UserModel::resetStateAll()
{
    // 组装sql
    char sql[1024] = {0};
    sprintf(sql, "UPDATE user SET state='offline' WHERE state='online'");
    // 连接mysqlserver并修改记录
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
