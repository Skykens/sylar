/*************************************************************************
	> File Name: macro.h
	> Author: 
	> Mail: 
	> Created Time: 2020年04月21日 星期二 15时50分09秒
    常用宏的封装
 ************************************************************************/

#ifndef _MACRO_H
#define _MACRO_H
#include <string.h>
#include <assert.h>
#include "log.h"
#include "util.h"

//编译器的优化
#if defineed __GNUC__ || defined __llvm 

#define SKYLU_LIKELY(x) __builtin_expect(!!(x),1)
#define SKYLU_UNLIKELY(x) __builtin_expect(!!(x),0)

#else 

#define SKYLU_LIKELY(x) (x)
#define SKYLU_UNLIKELY(x) (x)
#endif 

#define SKYLU_ASSERT(x)   \
        if(SKYLU_UNLIKELY(!(x))) { \
            SKYLU_LOG_ERROR(SKYLU_LOG_ROOT())<< "ASSERTION: " #x\
                << " \n backtrace : \n" \
                <<skylu::BacktraceToString(100,2,"     ");assert(x); \
                }

#define SKYLU_ASSERT2(x,w) \
        if(SKYLU_UNLIKELY(!(x))) {  \ 
            SKYLU_LOG_ERROR(SKYLU_LOG_ROOT())<<"ASSERTION: " #x\
            <<"\n"<< w \
            <<"\nbacktrance: \n" \
            <<skylu::BacktraceToString(100,2,"    ");assert(x); \
            }



#endif
