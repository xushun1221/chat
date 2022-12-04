/*
 *  @Filename : Redis.cc
 *  @Description : Implementation of Redis
 *  @Datatime : 2022/12/04 17:23:17
 *  @Author : xushun
 */

#include "Redis.hh"
#include <iostream>

Redis::Redis() : publishContext_(nullptr), subscribeContext_(nullptr)
{
}
Redis::~Redis()
{
    if (publishContext_ != nullptr)
    {
        redisFree(publishContext_);
    }
    if (subscribeContext_ != nullptr)
    {
        redisFree(subscribeContext_);
    }
}

bool Redis::connect()
{
    // 发布
    publishContext_ = redisConnect("127.0.0.1", 6379);
    if (nullptr == publishContext_)
    {
        std::cerr << "connect redis fail!" << std::endl;
        return false;
    }
    // 订阅
    subscribeContext_ = redisConnect("127.0.0.1", 6379);
    if (nullptr == subscribeContext_)
    {
        std::cerr << "connect redis fail!" << std::endl;
        return false;
    }
    // 单独的线程中 监听通道上的时间 有消息就给业务层上报
    std::thread t([&]()
                  { observerChannelMessage(); });
    t.detach();
    return true;
}

bool Redis::publish(uint32_t channel, std::string message)
{
    redisReply *reply = (redisReply *)redisCommand(publishContext_, "PUBLISH %ld %s", channel, message.c_str());
    if (nullptr == reply)
    {
        std::cerr << "publish command fail!" << std::endl;
        return false;
    }
    freeReplyObject(reply);
    return true;
}

bool Redis::subscribe(uint32_t channel)
{
    // redisCommand == redisAppendCommand + redisBufferWrite + redisGetReply
    // SUBSCRIBE命令本身会造成线程阻塞等待通道消息 这里只做订阅 不接收消息
    // 只发送命令 不等待redisserver响应
    if (REDIS_ERR == redisAppendCommand(subscribeContext_, "SUBSCRIBE %ld", channel))
    {
        std::cerr << "subscribe command fail!" << std::endl;
        return false;
    }
    // 循环发送缓冲区 直到数据发送完毕
    int done = 0;
    while (!done)
    {
        if (REDIS_ERR == redisBufferWrite(subscribeContext_, &done))
        {
            std::cerr << "subscribe command fail!" << std::endl;
            return false;
        }
    }
    return true;
}

bool Redis::unsubscribe(uint32_t channel)
{
    if (REDIS_ERR == redisAppendCommand(subscribeContext_, "UNSUBSCRIBE %ld", channel))
    {
        std::cerr << "unsubscribe command fail!" << std::endl;
        return false;
    }
    int done = 0;
    while (!done)
    {
        if (REDIS_ERR == redisBufferWrite(subscribeContext_, &done))
        {
            std::cerr << "unsubscribe command fail!" << std::endl;
            return false;
        }
    }
    return true;
}

void Redis::observerChannelMessage()
{
    redisReply *reply = nullptr;
    while (REDIS_OK == redisGetReply(subscribeContext_, (void **)&reply))
    {
        // 消息是一个三元素的数组
        // element[1]是channel element[2]是message
        if (reply != nullptr && reply->element[2] != nullptr && reply->element[2]->str != nullptr)
        {
            // 上报给业务层
            notifyMessageHandler_(atoi(reply->element[1]->str), reply->element[2]->str);
        }
        freeReplyObject(reply);
    }
    std::cerr << "observerChannelMessage quit" << std::endl;
}

void Redis::initNotifyHandler(std::function<void(uint32_t, std::string)> fn)
{
    notifyMessageHandler_ = fn;
}