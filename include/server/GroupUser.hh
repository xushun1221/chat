/*
 *  @Filename : GroupUser.hh
 *  @Description : ORM GroupUser
 *  @Datatime : 2022/12/02 14:49:33
 *  @Author : xushun
 */
#ifndef __GROUPUSER_HH_
#define __GROUPUSER_HH_

#include "User.hh"

// groupuser的ORM类 继承自User
class GroupUser : public User
{
public:
    std::string getRole() { return role_; }
    void setRole(std::string role) { role_ = role; }

private:
    std::string role_;
};

#endif // __GROUPUSER_HH_