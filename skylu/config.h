/*************************************************************************
	> File Name: config.h
	> Author: 
	> Mail: 
	> Created Time: 2020年04月16日 星期四 08时14分22秒
    配置 模块
 ************************************************************************/

#ifndef _CONFIG_H
#define _CONFIG_H
#include <memory>
#include <string> 
#include <sstream>
#include <boost/lexical_cast.hpp>
#include <exception>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include "thread.h"
#include "log.h"
#include "util.h"
#include "yaml-cpp/yaml.h"

namespace skylu{
class ConfigVarBase{
public: 
    typedef std::shared_ptr<ConfigVarBase> ptr;
    ConfigVarBase(const std::string & name,const std::string &desc = "")
        :m_name(name),
        m_description(desc)
    {
        std::transform(m_name.begin(),m_name.end(),m_name.begin(),::tolower);
    }
    
    virtual ~ConfigVarBase()=default;

    const std::string& getName() const {return m_name;}

    const std::string& getDescription() const {return m_description;} 

    virtual std::string toString()=0;   // 转换程字符串

    virtual bool fromString(const std::string& val)=0; //从字符串初始化值
    
    virtual std::string getTypeName() const =0; //返回配置参数的类型名称



protected:
    std::string m_name;
    std::string m_description;
};

//类型转换模板  是to_string /atod/atoi 等类型转换的替代方法
template<class F,class T>
class LexicalCast{
public:
    T operator() (const F& v)
    {
        return boost::lexical_cast<T>(v); //类型转换，失败会抛出异常
    }


};


//将读取到的YML string 转换成T 并存放在容器里面 
template<class T> 
class LexicalCast<std::string,std::vector<T> >{
public:
    std::vector<T> operator()(const std::string& v)
    {
        YAML::Node node = YAML::Load(v);
        std::vector<T> vec;
        std::stringstream ss;
        for(size_t i=0; i < node.size(); ++i)
        {
            ss.str("");
            ss << node[i];
            vec.push_back(LexicalCast<std::string,T>()(ss.str()));

        }
        return vec;

    }
        

};

//重载函数
template<class T>
class LexicalCast<std::vector<T> , std::string>{
public:
    std::string operator()(const std::vector<T>& v)
    {
        YAML::Node node(YAML::NodeType::Sequence);
        for(auto& i : v)
        {
            node.push_back(YAML::Load(LexicalCast<T,std::string>()(i)));

        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }

};




//将读取到的YML string 转换成T 并存放在容器里面 
template<class T> 
class LexicalCast<std::string,std::list<T> >{
public:
    std::list<T> operator()(const std::string& v)
    {
        YAML::Node node = YAML::Load(v);
        std::list<T> vec;
        std::stringstream ss;
        for(size_t i=0; i < node.size(); ++i)
        {
            ss.str("");
            ss << node[i];
            vec.push_back(LexicalCast<std::string,T>()(ss.str()));

        }
        return vec;

    }
        

};

//重载函数
template<class T>
class LexicalCast<std::list<T> , std::string>{
public:
    std::string operator()(const std::list<T>& v)
    {
        YAML::Node node(YAML::NodeType::Sequence);
        for(auto& i : v)
        {
            node.push_back(YAML::Load(LexicalCast<T,std::string>()(i)));

        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }

};




//将读取到的YML string 转换成T 并存放在容器里面 
template<class T> 
class LexicalCast<std::string,std::set<T> >{
public:
    std::set<T> operator()(const std::string& v)
    {
        YAML::Node node = YAML::Load(v);
        std::set<T> vec;
        std::stringstream ss;
        for(size_t i=0; i < node.size(); ++i)
        {
            ss.str("");
            ss << node[i];
            vec.insert(LexicalCast<std::string,T>()(ss.str()));

        }
        return vec;

    }
        

};

//重载函数
template<class T>
class LexicalCast<std::set<T> , std::string>{
public:
    std::string operator()(const std::set<T>& v)
    {
        YAML::Node node(YAML::NodeType::Sequence);
        for(auto& i : v)
        {
            node.push_back(YAML::Load(LexicalCast<T,std::string>()(i)));

        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }

};



//将读取到的YML string 转换成T 并存放在容器里面 
template<class T> 
class LexicalCast<std::string,std::unordered_set<T> >{
public:
    std::unordered_set<T> operator()(const std::string& v)
    {
        YAML::Node node = YAML::Load(v);
        std::unordered_set<T> vec;
        std::stringstream ss;
        for(size_t i=0; i < node.size(); ++i)
        {
            ss.str("");
            ss << node[i];
            vec.insert(LexicalCast<std::string,T>()(ss.str()));

        }
        return vec;

    }
        

};

//重载函数
template<class T>
class LexicalCast<std::unordered_set<T> , std::string>{
public:
    std::string operator()(const std::unordered_set<T>& v)
    {
        YAML::Node node(YAML::NodeType::Sequence);
        for(auto& i : v)
        {
            node.push_back(YAML::Load(LexicalCast<T,std::string>()(i)));

        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }

};




//将读取到的YML string 转换成T 并存放在容器里面 
template<class T> 
class LexicalCast<std::string,std::map<std::string,T> >{
public:
    std::map<std::string, T> operator()(const std::string& v)
    {
        YAML::Node node = YAML::Load(v);
        std::map<std::string,T>  vec;
        std::stringstream ss;
        for(auto it = node.begin(); it != node.end();++it)
        {
            ss.str("");
            ss<<it->second;
            vec.insert(std::make_pair(it->first.Scalar(),LexicalCast<std::string,T>()(ss.str())));
        }
        return vec;

    }
        

};

//重载函数
template<class T>
class LexicalCast<std::map<std::string,T> , std::string>{
public:
    std::string operator()(const std::map<std::string,T>& v)
    {
        YAML::Node node(YAML::NodeType::Map);
        for(auto& i : v)
        {
            node[i.first] = YAML::Load(LexicalCast<T,std::string>()(i.second));


        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }

};




//将读取到的YML string 转换成T 并存放在容器里面 
template<class T> 
class LexicalCast<std::string,std::unordered_map<std::string,T> >{
public:
    std::map<std::string, T> operator()(const std::string& v)
    {
        YAML::Node node = YAML::Load(v);
        std::unordered_map<std::string,T>  vec;
        std::stringstream ss;
        for(auto it = node.begin(); it != node.end();++it)
        {
            ss.str("");
            ss<<it->second;
            vec.insert(std::make_pair(it->first.Scalar(),LexicalCast<std::string,T>()(ss.str())));
        }
        return vec;

    }
        

};

//重载函数
template<class T>
class LexicalCast<std::unordered_map<std::string,T> , std::string>{
public:
    std::string operator()(const std::unordered_map<std::string,T>& v)
    {
        YAML::Node node(YAML::NodeType::Map);
        for(auto& i : v)
        {
            node[i.first] = YAML::Load(LexicalCast<T,std::string>()(i.second));


        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }

};

//配置参数模板的子类  保存对应类型的参数值
//   T ： 参数的具体类型
//  FromStr: 从std::string 转换成T 类型的仿函数
//  ToStr : 从T 转换成std::string 的仿函数
//  std::string 为YAML格式的字符串

template<class T ,class FromStr = LexicalCast<std::string,T>
        ,class ToStr = LexicalCast<T,std::string> >
class ConfigVar : public ConfigVarBase{
public: 
    typedef RWMutex RWMutexType;
    typedef std::shared_ptr<ConfigVar> ptr;
    typedef std::function<void(const T &old_value,const T & new_value)> on_change_cb;

    ConfigVar(const std::string& name,const T & default_value,const std::string & description = "")
    :ConfigVarBase(name,description),
    m_val(default_value)
    {
        
    }
    std::string toString() override 
    {
        try{
            RWMutexType::ReadLock lock(m_mutex);
            return ToStr()(m_val);

        }catch(std::exception& e)
        {
            SKYLU_LOG_ERROR(SKYLU_LOG_ROOT())<< " ConfigVar::toString exception"
            <<e.what() << " convert: "<<TypeToName<T>() << " to string"<<" name = "<<m_name;
        }
        return "";
    }

    bool fromString(const std::string & val)override
    {
        
        try{
            setValue(FromStr()(val));
            return true;
        }catch(std::exception & e)
        {
            SKYLU_LOG_ERROR(SKYLU_LOG_ROOT()) << "ConfigVar::fromString exception "
            << e.what() << " convert: string to " << TypeToName<T>()
            << " name=" << m_name
            << " - " << val;
        }
        return false;
    }

    const T getValue()
    {
        RWMutexType::ReadLock lock(m_mutex);
        return m_val;
    }

    void  setValue(const T & v)
    {
        {
            RWMutexType::ReadLock lock(m_mutex);
            if(v==m_val)
            {
                return;
            }
            for(auto & i: m_cbs)
            {
                i.second(m_val,v); //触发修改值的回调函数  回调函数里面不能有参数的锁
            }

        }
        RWMutexType::WriteLock lock(m_mutex);
        m_val = v;
    }

    std::string getTypeName() const override 
    {
        return TypeToName<T>();

    }


    uint64_t addListener(on_change_cb cb)
    {
        static uint64_t fun_id=0;
        RWMutexType::WriteLock lock(m_mutex);
        ++fun_id;
        m_cbs[fun_id]=cb;
        return fun_id;
        
    }

    void delListener(uint64_t key)
    {
        RWMutexType::WriteLock lock(m_mutex);
        m_cbs.erase(key);
    }

    on_change_cb getListener(uint64_t key)
    {
        RWMutexType::WriteLock lock(m_mutex);
        auto it=m_cbs.find(key);
        return it==m_cbs.end()?nullptr:it->second;
    }

    void clearListener()
    {
        RWMutexType::WriteLock lock(m_mutex);
        m_cbs.clear();
    }


private:
    RWMutexType m_mutex;
    T m_val;
    std::unordered_map<uint64_t,on_change_cb> m_cbs; //回调函数集合
};
//ConfigVar的管理类
class Config{
public:
    typedef std::unordered_map<std::string,ConfigVarBase::ptr>ConfigVarMap;
    typedef RWMutex RWMutexType;


    /*
     * 获取/创建对应名称的配置参数
     * 存在即返回，不存在就创建从那数并用default_value赋值
     * return:如果参数名参在但是类型不匹配则返回nullptr，存在则返回配置参数
     */

    template<class T> 
    static typename ConfigVar<T>::ptr Lookup(const std::string& name,
                                    const T default_value,const std::string &description = "")
    {
        RWMutexType::WriteLock lock(GetMutex());
        auto it=GetDatas().find(name);
        if(it != GetDatas().end())
        {
            auto tmp = std::dynamic_pointer_cast<ConfigVar<T> >(it->second);
            if(tmp)
            {
                SKYLU_LOG_INFO(SKYLU_LOG_ROOT())<< " Look up name = "<<name << "exits";
                return tmp;
            }else{
                SKYLU_LOG_ERROR(SKYLU_LOG_ROOT())<< "Lookup name ="<<name <<" exists ";
                return nullptr;
            }

        }
        if(name.find_first_not_of("abcdefghikjlmnopqrstuvwxyz._0123456789")!=std::string::npos)
        {
            SKYLU_LOG_ERROR(SKYLU_LOG_ROOT())<<"Lookup name invalid "<<name;
            throw std::invalid_argument(name);
        }
       typename ConfigVar<T>::ptr v(new ConfigVar<T>(name,default_value,description));
        GetDatas()[name]= v;
        return v;
    }
    /* 查找配置参数
     * name: 配置参数名称
     * return : 返回配置参数
     *
     */
    template<class T>
    static typename ConfigVar<T>::ptr Lookup(const std::string& name)
    {
        RWMutexType::ReadLock lock(GetMutex());
        auto it = GetDatas().find(name);
        if(it == GetDatas().end())
        {
            return nullptr;
        }
        return std::dynamic_pointer_cast<ConfigVar<T> >(it->second);

    }
    static void LoadFromYaml(const YAML::Node& root);

    static void LoadFromConfDir(const std::string& path,bool force = false);

    static ConfigVarBase::ptr LookupBase(const std::string& name);

    static void Visit(std::function<void(ConfigVarBase::ptr)> cb);

private:

    static ConfigVarMap& GetDatas()
    {
        static ConfigVarMap s_datas;
        return s_datas;

    }


    static RWMutexType& GetMutex()
    {
        static RWMutexType s_mutex;
        return s_mutex;
    }
    

};




}
#endif
