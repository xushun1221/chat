/*
 *  @Filename : Redis.hh
 *  @Description : Definition of Redis
 *  @Datatime : 2022/12/04 17:01:48
 *  @Author : xushun
 */
#ifndef __REDIS_HH_
#define __REDIS_HH_

#include <hiredis/hiredis.h>
#include <functional>
#include <thread>

class Redis
{
public:
    Redis();
    ~Redis();

    // 连接redis服务器
    bool connect();
    // 向指定channel发布消息
    bool publish(uint32_t channel, std::string message);
    // 向指定channel订阅消息
    bool subscribe(uint32_t channel);
    // 向指定channel取消订阅消息
    bool unsubscribe(uint32_t channel);
    // 在独立线程中接收该订阅通道中的消息
    void observerChannelMessage();
    // 初始化向业务层上报通道消息的回调对象
    void initNotifyHandler(std::function<void(uint32_t, std::string)> fn);

private:
    // hiredis同步上下文对象 相当于一个redisclient
    // 负责publish消息
    redisContext *publishContext_;
    // subscribe消息
    redisContext *subscribeContext_;
    // 回调方法 收到订阅的消息 给服务层上报
    std::function<void(uint32_t, std::string)> notifyMessageHandler_;
};

#endif // __REDIS_HH_