/*
 *  @Filename : public.hh
 *  @Description : server and client public
 *  @Datatime : 2022/11/27 16:29:12
 *  @Author : xushun
 */
#ifndef __PUBLIC_HH_
#define __PUBLIC_HH_

// 消息类型
enum MsgType
{
    LOGIN_MSG = 1,      // 登录消息
    LOGIN_ACK,          // 登录响应消息
    LOGOUT_MSG,         // 登出消息
    REG_MSG,            // 注册消息
    REG_ACK,            // 注册响应消息
    PEER_CHAT_MSG,      // 一对一聊天消息
    ADD_FRIEND_MSG,     // 添加好友消息
    CREATE_GROUP_MSG,   // 创建群组
    JOIN_GROUP_MSG,     // 加入群组
    GROUP_CHAT_MSG,     // 群聊消息
};

#endif // __PUBLIC_HH_