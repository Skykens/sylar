cmake_minimum_required(VERSION 3.0)  #指定使用的版本
project(demo)   # 项目名称 一般和项目文件夹相对应 
set(CMAKE_VERBOSE_MAKEFILE ON)  # 显示编译的过程
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O0 -g -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function")   #设置 编译选项  set  设置变量的意思

#set(LIB_SRC skylu/log.cc 
#    skylu/env.cc 
#    skylu/mutex.cc
#    skylu/thread.cc 
#    skylu/util.cc 
#	)  #设置变量 

aux_source_directory(test/ DIR_SRCS)
add_subdirectory(skylu)
add_executable(Demo tests/test.cc)
target_link_libraries(Demo pthread yaml-cpp skylu)

include_directories(${PROJECT_SOURCE_DIR}/skylu) #添加头文件的搜索路径 也就是-I
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)  #可执行文件的输出路径
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib) #设置编译后动态库的路径

