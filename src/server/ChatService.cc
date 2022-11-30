/*
 *  @Filename : ChatService.cc
 *  @Description : Implementation of ChatService
 *  @Datatime : 2022/11/27 16:50:04
 *  @Author : xushun
 */

#include "ChatService.hh"
#include "public.hh"
#include <muduo/base/Logging.h>
#include <string>
#include <map>

using namespace std::placeholders;
using namespace muduo;

ChatService::ChatService()
{
    msgHandlerMap_[LOGIN_MSG] = std::bind(&ChatService::login, this, _1, _2, _3);
    msgHandlerMap_[REG_MSG] = std::bind(&ChatService::registr, this, _1, _2, _3);
    msgHandlerMap_[PEER_CHAT_MSG] = std::bind(&ChatService::peerChat, this, _1, _2, _3);
    msgHandlerMap_[ADD_FRIEND_MSG] = std::bind(&ChatService::addFriend, this, _1, _2, _3);
}

ChatService *ChatService::instance()
{
    static ChatService service;
    return &service;
}

MsgHandler ChatService::getHandler(int msgType)
{
    auto itr = msgHandlerMap_.find(msgType);
    // 错误的msgType 不存在对应的消息处理回调
    if (itr == msgHandlerMap_.end())
    {
        // 返回一个空的处理器 不做任何处理  打印错误日志
        return [=](const TcpConnectionPtr &conn, json &js, Timestamp time)
        {
            LOG_ERROR << "MSG_TYPE: " << msgType << " cannot find a handler!";
        };
    }
    return itr->second;
}

void ChatService::reset()
{
    // 重置所有在线用户的在线状态
    userModel_.resetStateAll();
}

void ChatService::login(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    uint32_t id = js["id"].get<uint32_t>();
    std::string password = js["password"];

    User user = userModel_.queryById(id);
    if (user.getId() != 0 && user.getPassword() == password)
    {
        // id存在并且密码正确
        if (user.getState() == "online")
        {
            // 该用户已登录 不允许重复登录
            json response;
            response["msg_type"] = LOGIN_ACK;
            response["err_no"] = 2;
            response["err_msg"] = "already logged in, donot login again";
            conn->send(response.dump());
        }
        else
        {
            // 该用户未登录 登录成功

            // 1. 登录成功需要记录该用户的连接信息
            {
                std::lock_guard<std::mutex> lock(connectionMutex);
                userConnectionMap_.insert({id, conn});
            }

            // 2. 登录成功需要更新用户状态信息
            user.setState("online");
            userModel_.updateState(user);

            // 3. 返回响应信息
            json response;
            response["msg_type"] = LOGIN_ACK;
            response["err_no"] = 0;
            response["id"] = user.getId();
            response["name"] = user.getName(); // 返回用户的信息

            // 4. 如有离线消息 在响应消息中捎带 立即推送
            std::vector<std::string> vecOffMsg = offlineMessageModel_.queryById(id);
            if (!vecOffMsg.empty())
            {
                response["offline_msg"] = vecOffMsg;
                // 读取完离线消息后 应该将离线消息表中的数据删除 否则下次登录还会推送
                offlineMessageModel_.remove(id);
            }

            // 5. 如果该用户好友列表不空 推送给他
            std::vector<User> vecFrd = friendModel_.queryById(id);
            if (!vecFrd.empty())
            {
                std::vector<json> frdList;
                frdList.reserve(vecFrd.size());
                for (auto frd : vecFrd)
                {
                    json fjs;
                    fjs["id"] = frd.getId();
                    fjs["name"] = frd.getName();
                    fjs["state"] = frd.getState();
                    frdList.push_back(fjs);
                }
                response["friend_list"] = frdList;
            }

            conn->send(response.dump());
        }
    }
    else
    {
        // 登录失败
        json response;
        response["msg_type"] = LOGIN_ACK;
        response["err_no"] = 1;
        response["err_msg"] = "wrong id or password";
        conn->send(response.dump());
    }
}

void ChatService::registr(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    std::string name = js["name"];
    std::string password = js["password"];

    User user;
    user.setName(name);
    user.setPassword(password);

    if (userModel_.insert(user))
    {
        // 注册成功 发送响应消息给用户并且告知用户id
        json response;
        response["msg_type"] = REG_ACK;
        response["err_no"] = 0;
        response["id"] = user.getId();
        conn->send(response.dump());
    }
    else
    {
        // 注册失败
        json response;
        response["msg_type"] = REG_ACK;
        response["err_no"] = 1;
        conn->send(response.dump());
    }
}

void ChatService::clientCloseException(const TcpConnectionPtr &conn)
{
    // 某个连接异常下线 找到该用户id删除连接信息 并更改在线状态
    User user;
    {
        std::lock_guard<std::mutex> lock(connectionMutex);
        for (auto itr = userConnectionMap_.begin(); itr != userConnectionMap_.end(); ++itr)
        {
            if (itr->second == conn)
            {
                user.setId(itr->first);
                // 移除连接信息
                userConnectionMap_.erase(itr);
                break;
            }
        }
    }
    if (user.getId() != 0)
    {
        // 找到了连接对应的id 更改在线状态
        user.setState("offline");
        userModel_.updateState(user);
    }
}

void ChatService::peerChat(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    uint32_t peerid = js["to"].get<uint32_t>();
    // 目标用户是否在线
    std::unordered_map<uint32_t, TcpConnectionPtr>::iterator itr;
    {
        std::lock_guard<std::mutex> lock(connectionMutex);
        itr = userConnectionMap_.find(peerid);
        // 在线消息转发应该在临界区内操作 因为出临界区后连接可能被释放
        if (itr != userConnectionMap_.end())
        {
            // 在线 转发消息
            itr->second->send(js.dump());
            return;
        }
    }
    // 离线 存储离线消息
    offlineMessageModel_.insert(peerid, js.dump());
}

void ChatService::addFriend(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    uint32_t userid = js["userid"].get<uint32_t>();
    uint32_t friendid = js["friendid"].get<uint32_t>();
    // 存储好友信息
    if (userid != friendid)
    {
        friendModel_.insert(userid, friendid);
    }
}
