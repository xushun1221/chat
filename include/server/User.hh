/*
 *  @Filename : User.hh
 *  @Description : ORM User
 *  @Datatime : 2022/11/28 14:30:40
 *  @Author : xushun
 */
#ifndef __USER_HH_
#define __USER_HH_

#include <string>

// user表的ORM类
class User
{
public:
    User(uint32_t id = 0, std::string name = "", std::string password = "", std::string state = "offline")
        : id_(id), name_(name), password_(password), state_(state)
    {
    }
    void setId(uint32_t id) { id_ = id; }
    void setName(std::string name) { name_ = name; }
    void setPassword(std::string password) { password_ = password; }
    void setState(std::string state) { state_ = state; }
    uint32_t getId() { return id_; }
    std::string getName() { return name_; }
    std::string getPassword() { return password_; }
    std::string getState() { return state_; }

private:
    uint32_t id_;
    std::string name_;
    std::string password_;
    std::string state_;
};

#endif // __USER_HH_