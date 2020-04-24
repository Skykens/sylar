/*************************************************************************
	> File Name: scheduler.h
	> Author: 
	> Mail: 
	> Created Time: 2020年04月21日 星期二 17时41分32秒
    协程调度封装
 ************************************************************************/

#ifndef _SCHEDULER_H
#define _SCHEDULER_H
#include <memory>
#include <vector>
#include <list>
#include <iostream>
#include "nocopyable.h"
#include "fiber.h"
#include "thread.h"
#include "mutex.h"
class Scheduler{
public:
    typedef std::shared_ptr<Scheduler> ptr;
    typedef Mutex MutexType;

    Scheduler(size_t threads= 1,bool use_caller = true,const std::string& name= "");
    virtual ~Scheduler();

    const std::string& getName()const {return m_name;}

    static Scheduler* GetThis();

    static Fiber* GetMainFiber();

    void start();

    void stop();

    template<class FiberOrCb>
    void schedule(FiberOrCb fc,int thread = -1)
    {
        bool need_tickle = false;
        {
            MutexType::Lock lock(m_mutex);
            need_tickle =  scheduleNoLock(fc,thread); //无锁调度
        }
        if(need_tickle)
        {
            //通知协程调度器有任务了
            tickle();
        }
    }
    /*
     *
     * @brief 批量调度协程
     *
     * @param[in] begin 开始 
     * @param[in] end 结束
     *
     */

    template<class InputIterator>
    void schedule(InputIterator begin,InputIterator end)
    {
        bool need_tickle = false;
        {
            MutexType::Lock lock(m_mutex);
            while(begin != end){
                need_tickle = scheduleNoLock(&*begin, -1) ||need_tickle;
                ++begin;
            }

        }
        if(need_tickle)
            tickle();
    }


    void switchTo(int thread = -1);
    std::ostream& dump(std::ostream& os);

protected:

    /*
     * @brief 通知协程调度器有任务了
     *
     */
    virtual void tickle();

    /*
     *
     * @brief 协程调度函数
     *
     */
    void run();

    /*
     *
     * @brief 返回是否可以停止
     */
    virtual bool stopping();


    /*
     * @brief 协程无任务调度时执行idle协程
     *
     */
    virtual void idle();


    /*
     * @brief 设置当前的协程调度器
     *
     */
    void setThis();

    /*
     *
     * @brief 是否有空闲线程
     *
     */
    bool  hasIdleThreads(){return m_idleThreadCount>0;}

private:
    /*
    * 调度器实际调用的方法，用来无锁的启动调度
    *
    */
    template<class FiberOrCb>
    bool scheduleNoLock(FiberOrCb fc,int thread)
    {
        bool need_tickle = m_fibers.empty();
        FiberAndThread ft(fc,thread);
        if(ft.fiber || ft.cb ){
            m_fibers.push_back(ft);
        }
        return need_tickle;
    }


private:
    //协程 线程 二元组
    struct FiberAndThread{
        Fiber::ptr fiber;  //协程
        std::function<void()> cb; //线程执行函数
        int thread; //线程id

        FiberAndThread(Fiber::ptr f,int thr)
            :fiber(f),thread(thr){}


        FiberAndThread(Fiber::ptr *f,int thr)
             :thread(thr){
            fiber.swap(*f);
        }

        FiberAndThread(std::function<void()> f,int thr)
            :cb(f),thread(thr){}

        FiberAndThread(std::function<void()> *f,int thr)
            :thread(thr){
            cb.swap(*f);
            
        }

        /*
         *
         * @brief 无参数
         *
         */
        FiberAndThread(): thread(-1){}

        void reset()
        {
            fiber=nullptr;
            cb = nullptr;
            thread = -1;
        }


    };



private:
    MutexType m_mutex;
    std::vector<Thread::ptr> m_threads; //线程池
    std::list<FiberAndThread> m_fibers; //待执行的协程队列
    Fiber::ptr m_rootFiber;
    std::string m_name;

protected:
    std::vector<int> m_threadIds;
    size_t m_threadCount = 0; //线程数量
    std::atomic<size_t> m_activeThreadCount = {0}; //工作线程数
    std::atomic<size_t> m_idleThreadCount = {0}; //空闲线程数量
    bool m_stopping = true; //是否停止
    bool m_autoStop = false; //自动停止
    int m_rootThread = 0; //主线程id

};

class SchedulerSwitcher: public Nocopyable{
public:
    SchedulerSwitcher(Shceduler * target = nullptr);
    ~SchedulerSwitcher();

private:
    Scheduler * m_caller;
    

};
#endif
