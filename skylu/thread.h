/*************************************************************************
	> File Name: thread.h
	> Author: 
	> Mail: 
	> Created Time: 2020年04月13日 星期一 10时50分26秒
    对线程的封装
 ************************************************************************/

#ifndef _THREAD_H
#define _THREAD_H
#include "nocopyable.h"
#include "mutex.h"
namespace skylu{
class Thread : Nocopyable{
public:
    typedef std::shared_ptr<Thread>  ptr;
    Thread(std::function<void()>cb, const std::string& name);
    ~Thread();
    pid_t getId()const {return m_id;}
    const std::string& getName()const {return m_name;}
    void join();
    static Thread* GetThis();
    static const std::string & GetName();
    static void SetName(const std::string& name);
private:
    static void * run(void *arg);


private:
    pid_t m_id = -1;
    pthread_t m_thread = 0;
    std::function<void()> m_cb;
    std::string m_name;
    skylu::Semaphore m_semaphore;

};
}
#endif
