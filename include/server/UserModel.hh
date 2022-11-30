/*
 *  @Filename : UserModel.hh
 *  @Description : UserModel Definition
 *  @Datatime : 2022/11/28 14:36:46
 *  @Author : xushun
 */
#ifndef __USERMODEL_HH_
#define __USERMODEL_HH_

#include "User.hh"

// user表的数据操作类
class UserModel
{
public:
    // user表的插入方法
    bool insert(User &user);
    // 根据id查询记录
    User queryById(uint32_t id);
    // 更新用户状态信息
    bool updateState(User &user);
    // 重置所有用户的状态信息
    bool resetStateAll();
};

#endif // __USERMODEL_HH_