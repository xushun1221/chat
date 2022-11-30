/*
 *  @Filename : OfflineMessageModel.hh
 *  @Description : Definition of OfflineMessageModel
 *  @Datatime : 2022/11/30 10:20:47
 *  @Author : xushun
 */
#ifndef __OFFLINEMESSAGEMODEL_HH_
#define __OFFLINEMESSAGEMODEL_HH_

#include <string>
#include <vector>

// offlinemessage表的数据操作类
class OfflineMessageModel
{
public:
    // 存储一条用户的离线消息
    bool insert(uint32_t userid, std::string msg);
    // 删除用户所有的离线消息
    bool remove(uint32_t userid);
    // 查询用户所有的离线消息
    std::vector<std::string> queryById(uint32_t userid);
};

#endif // __OFFLINEMESSAGEMODEL_HH_