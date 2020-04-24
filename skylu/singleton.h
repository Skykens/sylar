/*************************************************************************
	> File Name: singleton.h
	> Author: 
	> Mail: 
	> Created Time: 2020年04月17日 星期五 10时57分21秒
    单例模式的封装
 ************************************************************************/

#ifndef _SINGLETON_H
#define _SINGLETON_H
#include <memory>
namespace skylu{


/*
 * 单例模式的封装 
 * T ： 类型
 * X ： 为了创建多个示例对应的tag
 * N ： 同一个tag创造多个实例索引
 */
template<class T,class X =void ,int N = 0>
class Singleton{
public: 
    //返回单例裸指针
    static T * GetInstance(){
        static T v;
        return &v;
    }
};
template<class T,class X = void,int N =0>
class SingletonPtr{
public:
    static std::shared_ptr<T> GetInstance(){
        static std::shared_ptr<T> v(new T);
        return v;
    }


        
};

    
        
}




#endif
