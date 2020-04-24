/*************************************************************************
	> File Name: env.h
	> Author: 
	> Mail: 
	> Created Time: 2020年04月17日 星期五 11时08分39秒
    保存和配置全局环境 和 config 交互得到YAML中存储的配置参数
 ************************************************************************/

#ifndef _ENV_H
#define _ENV_H
#include "singleton.h"
#include "thread.h"
#include <map>
#include <vector>

namespace skylu{
class Env{
public: 
    typedef RWMutex RWMutexType;
    bool init(int argc,char **argv);
    
    void add(const std::string& key,const std::string& val);
    bool has(const std::string& key);
    void del(const std::string& key);
    std::string get(const std::string&key,const std::string& default_value = "");

    void addHelp(const std::string& key,const std::string& desc);
    void removeHelp(const std::string& key);
    void printHelp();

    const std::string& getExe()const {return m_exe;}
    const std::string& getCwd()const{return m_cwd;}

    bool setEnv(const std::string& key,const std::string& val);
    std::string getEnv(const std::string& key,const std::string& default_value="");

    /* 
     * 获得绝对路径
     *
     */
    std::string getAbsolutePath(const std::string& path)const;
    std::string getAbsoluteWorkPath(const std::string& path)const;
    std::string getConfigPath();

private:
    RWMutexType m_mutex;
    std::map<std::string,std::string> m_args;
    std::vector<std::pair<std::string,std::string> > m_helps;

    std::string m_program;
    std::string m_exe;
    std::string m_cwd;



};
typedef skylu::Singleton<Env> EnvMgr;
}
#endif
