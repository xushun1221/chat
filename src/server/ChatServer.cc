/*
 *  @Filename : ChatServer.cc
 *  @Description : ChatServer Implementation
 *  @Datatime : 2022/11/27 15:38:53
 *  @Author : xushun
 */
#include "json.hpp"
#include "ChatServer.hh"
#include "ChatService.hh"
#include <functional>
#include <string>

using namespace std::placeholders;
using json = nlohmann::json;

ChatServer::ChatServer(EventLoop *loop, const InetAddress &listenAddr, const string &nameArg, uint32_t threadNum)
    : server_(loop, listenAddr, nameArg), loop_(loop)
{
    // register ConnectionCallback
    server_.setConnectionCallback(std::bind(&ChatServer::onConnection, this, _1));
    // register MessageCallback
    server_.setMessageCallback(std::bind(&ChatServer::onMessage, this, _1, _2, _3));
    // set thread number: 1 acceptor loop, threadNum - 1 event loop
    server_.setThreadNum(threadNum);
}

void ChatServer::start()
{
    // start server
    server_.start();
}

void ChatServer::onConnection(const TcpConnectionPtr &conn)
{
    // client close connection
    if (!conn->connected())
    {
        // 连接异常断开
        ChatService::instance()->clientCloseException(conn);
        conn->shutdown();
    }
}

void ChatServer::onMessage(const TcpConnectionPtr &conn, Buffer *buffer, Timestamp time)
{
    // get message
    std::string buf = buffer->retrieveAllAsString();
    // parse message data
    json js = json::parse(buf);
    // 获取消息对应的事件处理器
    auto msgHandler = ChatService::instance()->getHandler(js["msg_type"].get<int>());
    // 处理
    msgHandler(conn, js, time);
    // 解耦 业务代码 和 网络代码
}