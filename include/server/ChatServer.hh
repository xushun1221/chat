/*
 *  @Filename : ChatServer.hh
 *  @Description : ChatServer Definition
 *  @Datatime : 2022/11/27 15:29:18
 *  @Author : xushun
 */
#ifndef __CHATSERVER_HH_
#define __CHATSERVER_HH_

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>

using namespace muduo;
using namespace muduo::net;

class ChatServer
{
public:
    // init chatserver
    ChatServer(EventLoop *loop, const InetAddress &listenAddr, const string &nameArg, uint32_t threadNum);
    // start server
    void start();

private:
    // server class in muduo lib
    TcpServer server_;
    // mainloop
    EventLoop *loop_;

private:
    // callback: connection create and shutdown
    void onConnection(const TcpConnectionPtr &conn);
    // callback: user io event
    void onMessage(const TcpConnectionPtr &conn, Buffer *buffer, Timestamp time);
};

#endif // __CHATSERVER_HH_