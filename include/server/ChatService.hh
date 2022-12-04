/*
 *  @Filename : ChatService.hh
 *  @Description : Service Business of ChatServer
 *  @Datatime : 2022/11/27 16:16:46
 *  @Author : xushun
 */
#ifndef __CHATSERVICE_HH_
#define __CHATSERVICE_HH_

#include "json.hpp"
#include "Redis.hh"
#include "UserModel.hh"
#include "OfflineMessageModel.hh"
#include "FriendModel.hh"
#include "GroupModel.hh"
#include <muduo/net/TcpConnection.h>
#include <unordered_map>
#include <functional>
#include <mutex>

using namespace muduo;
using namespace muduo::net;
using json = nlohmann::json;

// 处理消息的回调类型
using MsgHandler = std::function<void(const TcpConnectionPtr &conn, json &js, Timestamp time)>;

// 业务类  单例
class ChatService
{
public:
    // 获取单例对象
    static ChatService *instance();
    // 获取消息对应的处理器
    MsgHandler getHandler(int msgType);
    // 服务器进程异常中止 业务重置方法
    void reset();
    // 处理Redis上报的消息 转发到目标客户端上
    void handleRedisSubscribeMessage(uint32_t id, std::string message);

    // 客户端异常退出处理
    void clientCloseException(const TcpConnectionPtr &conn);
    // 登录业务
    void login(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 登出业务
    void logout(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 注册业务
    void registr(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 一对一聊天业务
    void peerChat(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 添加好友业务
    void addFriend(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 创建群组业务
    void createGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 加入群组业务
    void joinGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 群组聊天业务
    void groupChat(const TcpConnectionPtr &conn, json &js, Timestamp time);

private:
    // 消息类型对应的业务处理方法
    std::unordered_map<uint32_t, MsgHandler> msgHandlerMap_;

    // 在线用户的通信连接
    std::unordered_map<uint32_t, TcpConnectionPtr> userConnectionMap_;
    // 保证userConnectionMap线程安全
    std::mutex connectionMutex_;

    // user表数据操作对象
    UserModel userModel_;
    // offlinemessage表数据操作对象
    OfflineMessageModel offlineMessageModel_;
    // friend表数据操作对象
    FriendModel friendModel_;
    // group和groupuser表数据操作对象
    GroupModel groupModel_;

    // Redis操作对象
    Redis redis_;

private:
    // 在构造中注册消息类型和其对应的回调操作
    ChatService();
};

#endif // __CHATSERVICE_HH_