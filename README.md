# chat

- xushun
- 2022/12/04

基于C++11的集群聊天服务器和客户端实现。

## 特性

- 基于C++11
- 支持单服务器、集群服务器灵活部署
- 可以工作在Nginx-TCP负载均衡环境下
- 支持基于Redis发布-订阅模式的服务器间通信

## 依赖

- GCC 8.3.1
- CMake 3.24.3
- MySQL 5.7.40
- Redis 6.2.7
- Nginx 1.12.2
- muduo 2.0.2
- JSON for Modern C++ 3.11.2

## 编译

```bash
mkdir build
cd build
cmake ..
make
```

## 运行

```
bin/chatserver [ip] [port]
```