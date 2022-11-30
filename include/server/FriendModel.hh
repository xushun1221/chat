/*
 *  @Filename : FriendModel.hh
 *  @Description : Definition of FriendModel
 *  @Datatime : 2022/11/30 14:49:57
 *  @Author : xushun
 */

#ifndef __FRIENDMODEL_HH_
#define __FRIENDMODEL_HH_

#include "User.hh"
#include <vector>

// friend表的数据操作类
class FriendModel
{
public:
    // 添加好友关系
    bool insert(uint32_t userid, uint32_t friendid);
    // 获取用户的好友列表
    std::vector<User> queryById(uint32_t userid);
};

#endif // __FRIENDMODEL_HH_