/*************************************************************************
	> File Name: util.h
	> Author: 
	> Mail: 
	> Created Time: 2020年04月06日 星期一 17时48分34秒
 ************************************************************************/

#ifndef _UTIL_H
#define _UTIL_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <stdint.h>
#include <vector>
#include <string>
#include <iomanip>
#include <iostream>
#include <pthread.h>
#include <cxxabi.h>
#include <execinfo.h>
#include <sys/time.h>
#include "log.h"
#include "fiber.h"

namespace skylu{


static skylu::Logger::ptr g_logger = SKYLU_LOG_NAME("system");
pid_t getThreadId(); //返回当前线程id
uint32_t getFiberId(); //返回当前协程id

template<class T>
const char* TypeToName() {
    static const char* s_name = abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, nullptr);
    return s_name;
};

uint64_t GetCurrentMS();
uint64_t GetCurrentUS();

class FSUtil{
public:
    static void ListAllFile(std::vector<std::string> &files,
            const std::string& path,
            const std::string& subfix);
};
    

static std::string demangle(const char * str);


/* @brief 调用backtrace 返回当前函数堆栈
 * @param[out] bt  存放结果
 * @param[in] size 最多存放多少层函数堆栈
 * @param[in] skip 跳过前几个堆栈
 */
void Backtrace(std::vector<std::string>&bt,int size,int skip);

std::string BacktraceToString(int size,int skip,const std::string& prefix);

}



#endif
