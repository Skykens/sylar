/*************************************************************************
	> File Name: mutex.h
	> Author: 
	> Mail: 
	> Created Time: 2020年04月13日 星期一 10时52分36秒
    对互斥锁的封装  基于RAII原则
 ************************************************************************/

#ifndef _MUTEX_H
#define _MUTEX_H
#include <thread>
#include <functional>
#include <memory>
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <atomic>
#include <list>
#include "nocopyable.h"
#include "fiber.h"

namespace skylu{

class Semaphore : Nocopyable{
public: 
    Semaphore(uint32_t count = 0);
    ~Semaphore();
    void wait();
    void notify();
private:
    sem_t m_semaphore;
    
};




//局部锁模板的实现
template<class T> 
class ScopedLockImpl{
public: 
    ScopedLockImpl(T& mutex)
    :m_mutex(mutex){
        m_mutex.lock();
        m_locked=true;
    }

    ~ScopedLockImpl(){
        unlock();
    }
    void lock(){
        if(!m_locked){
            m_mutex.lock();
            m_locked=true;
        }
    }
    void unlock(){
        if(m_locked){
            m_mutex.unlock();
            m_locked=false;
        }
    }
private: 
    T& m_mutex;
    bool m_locked;

};


//局部读锁 模板实现
template <class T>
class ReadScopedLockImpl{
public:
    ReadScopedLockImpl(T& mutex)
    :m_mutex(mutex){

        m_mutex.rdlock();
        m_locked = true;
    }

    ~ReadScopedLockImpl()
    {
        unlock();
    }

    void lock()
    {
        if(!m_locked){
            m_mutex.rdlock();
            m_locked = true;
        }
    }

    void unlock()
    {
        if(m_locked){
            m_mutex.unlock();
            m_locked = false;

        }
    }


private:
    T& m_mutex;
    bool m_locked;
        
};


//局部写锁模板
template<class T>
class WriteScopedLockImpl{
public:
    WriteScopedLockImpl(T& mutex)
        :m_mutex(mutex)
    {
        m_mutex.wrlock();
        m_locked = true;
    
    }
    
    void lock() //非阻塞上锁
    {
        if(!m_locked)
        {
            m_mutex.wrlock();

            m_locked=true;
        }
    }
    
    void unlock()
    {
        if(m_locked)
        {
            m_mutex.unlock();
            m_locked = false;
        }
    }



private:
    T& m_mutex;
    bool m_locked;
    
};
class Mutex : Nocopyable{
public: 
    typedef ScopedLockImpl<Mutex> Lock;  //RAII 原则  局部锁模板

    Mutex()
    {
        pthread_mutex_init(&m_mutex,nullptr);

    }

    ~Mutex(){
        pthread_mutex_destroy(&m_mutex);
    }

    void lock()
    {
        pthread_mutex_lock(&m_mutex);

    }

    void unlock()
    {
        pthread_mutex_unlock(&m_mutex);
    }

private:
    pthread_mutex_t m_mutex;
        
};


// 读写互斥锁
class RWMutex : Nocopyable{
public: 
    typedef ReadScopedLockImpl<RWMutex> ReadLock;
    typedef WriteScopedLockImpl<RWMutex> WriteLock;

    RWMutex()
    {
        pthread_rwlock_init(&m_lock,nullptr);

    }

    ~RWMutex()
    {
        pthread_rwlock_destroy(&m_lock);

    }

    void rdlock()
    {
        pthread_rwlock_rdlock(&m_lock);

    }

    void wrlock()
    {
        pthread_rwlock_wrlock(&m_lock);
    }


    void unlock()
    {
        pthread_rwlock_unlock(&m_lock);
    }




private:
    pthread_rwlock_t m_lock;
};

// 自旋锁

class Spinlock : Nocopyable {
public:
    typedef ScopedLockImpl<Spinlock> Lock;

    Spinlock()
    {
        pthread_spin_init(&m_mutex,0);

    }

    ~Spinlock()
    {
        pthread_spin_destroy(&m_mutex);
    }

    void lock()
    {
        pthread_spin_lock(&m_mutex);
    }


    void unlock()
    {
        pthread_spin_unlock(&m_mutex);
    }

private: 
    pthread_spinlock_t m_mutex;
    
};

//原子锁

class CASLock : Nocopyable {
public:
    typedef ScopedLockImpl<CASLock> Lock;

    CASLock()
    {
        m_mutex.clear();
    }


    ~CASLock(){
        
    }

    void lock()
    {
        while(std::atomic_flag_test_and_set_explicit(&m_mutex, std::memory_order_acquire));
    }

    void unlock()
    {
        std::atomic_flag_clear_explicit(&m_mutex, std::memory_order_release);

    }

    


private:
    volatile std::atomic_flag m_mutex;
};

class Scheduler;
class FiberSemaphore: Nocopyable{
public: 
    typedef Spinlock MutexType;
    FiberSemaphore(size_t initial_concurrency = 0);
    ~FiberSemaphore();
    bool tryWait();
    void wait();
    void notify();

    size_t getConcurrency() const {return m_concurrency;}
    void reset(){m_concurrency = 0;}



private:
    MutexType m_mutex;
    std::list<std::pair<Scheduler*,Fiber::ptr> m_waitters;
    size_t m_concurrency;
};

}
#endif
