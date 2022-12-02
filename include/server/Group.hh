/*
 *  @Filename : Group.hh
 *  @Description : ORM Group
 *  @Datatime : 2022/12/2 14:40:40
 *  @Author : xushun
 */
#ifndef __GROUP_HH_
#define __GROUP_HH_

#include "GroupUser.hh"
#include <string>
#include <vector>

// group表的ORM类
class Group
{
public:
    Group(uint32_t id = 0, std::string name = "", std::string desc = "")
        : id_(id), name_(name), desc_(desc)
    {
    }
    uint32_t getId() { return id_; }
    std::string getName() { return name_; }
    std::string getDesc() { return desc_; }
    std::vector<GroupUser> &getUsers() { return users_; }
    void setId(uint32_t id) { id_ = id; }
    void setName(std::string name) { name_ = name; }
    void setDesc(std::string desc) { desc_ = desc; }
private:
    uint32_t id_;
    std::string name_;
    std::string desc_;
    // 用于保存群组中成员信息的查询结果 给业务层使用
    std::vector<GroupUser> users_;
};

#endif // __GROUP_HH_