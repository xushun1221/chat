/*
 *  @Filename : main.cc
 *  @Description : ChatServer Entry
 *  @Datatime : 2022/11/27 15:53:16
 *  @Author : xushun
 */

#include "ChatServer.hh"
#include "ChatService.hh"
#include <signal.h>

// 服务器异常中止 重置用户在线状态
void resetHandler(int)
{
    ChatService::instance()->reset();
    exit(0);
}

int main(int argc, char **argv)
{

    // 捕获程序中止信号 重置用户在线状态信息
    signal(SIGINT, resetHandler);

    EventLoop loop;
    InetAddress addr("127.0.0.1", 6000);
    ChatServer server(&loop, addr, "ChatServer", 8);

    server.start();
    loop.loop();

    return 0;
}