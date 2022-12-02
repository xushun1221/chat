/*
 *  @Filename : GroupModel.hh
 *  @Description :
 *  @Datatime : 2022/12/02 15:00:17
 *  @Author : xushun
 */
#ifndef __GROUPMODEL_HH_
#define __GROUPMODEL_HH_

#include "Group.hh"

// group和groupuser表的数据操作类
class GroupModel
{
public:
    // 创建群组
    bool createGroup(Group &group);
    // 加入群组
    bool joinGroup(uint32_t userid, uint32_t groupid, std::string role);
    // 查询用户的所在的群组
    std::vector<Group> queryGroupsById(uint32_t userid);
    // 查询群组中的其他用户id 用于群聊天
    std::vector<uint32_t> queryGroupUsers(uint32_t userid, uint32_t groupid);
};

#endif // __GROUPMODEL_HH_