cmake_minimum_required(VERSION 3.0)
project(chat)

# 编译选项
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -g)

# 可执行文件输出目录
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 头文件搜索路径
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/server)
include_directories(${PROJECT_SOURCE_DIR}/thirdparty)

# add src
aux_source_directory(src/server SERVER_SRC_LIST)
aux_source_directory(src/client CLIENT_SRC_LIST)

# 添加可执行文件
add_executable(chatserver ${SERVER_SRC_LIST})
# 依赖库文件
target_link_libraries(chatserver muduo_net muduo_base pthread)
target_link_libraries(chatserver /usr/lib64/mysql/libmysqlclient.so)

add_executable(chatclient ${CLIENT_SRC_LIST})
target_link_libraries(chatclient pthread)