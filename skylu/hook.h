/*************************************************************************
	> File Name: hook.h
	> Author: 
	> Mail: 
	> Created Time: 2020年04月24日 星期五 10时27分55秒
    hook相关函数
 ************************************************************************/

#ifndef _HOOK_H
#define _HOOK_H
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

namespace skylu{

bool is_hook_enable();

void set_hook_enable(bool flag);



    



}


extern "C"{

typedef unsigned int (*sleep_fun)(unsigned int seconds);
extern sleep_fun sleep_f;

typedef int (*usleep_fun)(useconds_t usec);
extern usleep_fun usleep_f;


typedef 
    





}
#endif
