#include "util.h"
#include <iostream>
#include <execinfo.h>
#include <sys/time.h>
#include <stdio.h>
#include <cstddef>
#include <dirent.h>
#include <string.h>
namespace skylu{

pid_t getThreadId()
{
    return syscall(SYS_gettid);

}
uint32_t getFiberId()
{

    return 0;
}

uint64_t GetCurrentMS()
{
    struct timeval tv;
    gettimeofday(&tv,nullptr);
    return tv.tv_sec * 1000ul + tv.tv_usec / 1000;

}


uint64_t GetCurrentUS()
{
    struct timeval tv;
    gettimeofday(&tv,nullptr);
    return tv.tv_sec * 1000 * 1000ul + tv.tv_usec;
        
}

//重新结构获取到的符号连接
static std::string demangle(const char * str)
{
    size_t size = 0;
    int status = 0;
    std::string rt;
    rt.resize(256);
    if(1 == sscanf(str,"%*[^(]%*[^_]%255[^)+]",&rt[0])) {
        char * v =abi::__cxa_demangle(&rt[0],nullptr,&size,&status); 
        //输出当前
        if(v){
            std::string result(v);
            free(v);
            return result;
        }
    }
    if(1==sscanf(str,"%255s",&rt[0])){
        return return rt;
    }
    return str;


}


void Backtrace(std::vector<std::string>&bt,int size,int skip)
{
    void **array= (void **)malloc((sizeof(void*)*size));
    size_t s = ::backtrace(array,size);


    char ** strings = backtrace_symbols(array,s);
    if(strings == nullptr)
    {
        SKYLU_LOG_ERROR(g_logger)<< "backtrace_synbols error";
        return ;
    }

}


std::string BacktraceToString(int size,int skip,const std::string& prefix)
{
    std::vector<std::string> bt; 
    Backtrace(bt,size,skip);
    std::stringstream ss;
    for(auto i: bt)
    {
        ss<< prefix<< i<<std::endl;
    }
    return ss.str();
}


void FSUtil::ListAllFile(std::vector<std::string> &files,const std::string& path,const std::string& subfix)
{
    /*
    if(assess(path.c_str(),0)!=0)
    {
        return ;
    }*/
    DIR* dir = opendir(path.c_str());
    if(dir == nullptr)
    {
        return ;
    }
    struct dirent* dp =nullptr;
    while((dp=readdir(dir))!=nullptr)
    {
        if(dp->d_type ==DT_DIR){
            if(!strcmp(dp->d_name,".")||!strcmp(dp->d_name,".."))
                continue;
            ListAllFile(files,path+"/"+dp->d_name,subfix);//深度优先搜索
        }else if(dp->d_type == DT_REG){
        std::string filename(dp->d_name);
        if(subfix.empty())
        {
            files.push_back(path+"/"+filename);

        }else {
            if(filename.size()<subfix.size())
            {
                continue;
            }
            if(filename.substr(filename.length()-subfix.size())==subfix)
            {
                files.push_back(path+"/"+filename);
            } //匹配后缀
            }
        }
    }
    closedir(dir);

}

}
