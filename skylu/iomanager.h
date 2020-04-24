/*************************************************************************
	> File Name: iomanager.h
	> Author: 
	> Mail: 
	> Created Time: 2020年04月22日 星期三 09时37分52秒
 ************************************************************************/

#ifndef _IOMANAGER_H
#define _IOMANAGER_H
#include "scheduler.h"
#include "timer.h"
#include "mutex.h"
namespace skylu{
class IOManager : public Scheduler,public  TimerManager{
public:
    typedef std::shared_ptr<IOManager> ptr;
    typedef RWMutex RWMutexType;

    enum Event{
        NONE = 0x0,
        READ = 0x1,
        WRITE = 0x04
    };

private:
    /*
     *
     * @brief socket事件上下文类
     *
     */
    struct FdContext{
        typedef Mutex MutexType;
        struct EventContext{
            Scheduler*  scheduler = nullptr;

            Fiber::ptr fiber;
            
            std::function<void()> cb;

        };
        EventContext& getContext(Event event);

        void resetContext(EventContext& ctx);

        void triggerEvent(Event event);

        EventContext read; //读事件上下文
        EventContext write;
        int fd = 0;
        Event events = NONE; //当前的事件

        MutexType mutex;
    };

public:
    /*
     *
     * @brief 
     * @param [in] threads 线程数量
     * @param [in]  use_caller 是否将调用线程包含进去
     */
    IOManager(size_t threads = 1,bool use_caller = true,const std::string& name);

    ~IOManager();
    

    int addEvent(int fd,Event event,std::function<void()> cb = nullptr);

    bool delEvent(int fd,Event event);

    bool cancelEvent(int fd,Event event);

    bool cancelAll(int fd);

    static IOManager* GetThis();

protected: 
    void tickle() override;
    bool stopping() override;
    void idle() override;
    void TimerInsertedAtFront() override;

    void contextResize(size_t size);

    bool stopping(uint64_t& timeout);
private:
    int m_epfd = 0;
    int m_tickleFds[2];
    std::atomic<size_t>  m_pendingEventCount = {0};

    RWMutexType m_mutex;
    std::vector<FdContext*> m_fdContexts;

};
}
#endif
