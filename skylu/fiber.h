/*************************************************************************
	> File Name: fiber.h
	> Author: 
	> Mail: 
	> Created Time: 2020年04月13日 星期一 13时38分04秒
    基于腾讯开源的coctx_swap.S实现的一个协程 尚未实现共享栈功能
 ************************************************************************/

#ifndef _FIBER_H
#define _FIBER_H
#include <memory>
#include <functional>
#include <ucontext.h>



namespace skylu{


class Scheduler; //调度器

class Fiber : public std::enble_shared_from_this<Fiber>
{
    friend class Scheduler;
public:
    typedef std::shared_ptr<Fiber> ptr;

    //状态
    emum State{
        INIT, //初始化状态
        HOLD, //暂停
        EXEC,  //执行态
        TERM, //结束状态
        READY, //就绪态
        EXCEPT //异常
    };
private:
    //无参数构造函数，为了能让调度器可以生成一个默认的后续再绑定响应的cb
    //但是不能放在外界
    Fiber();
public:
    //use_caller : 是否在MainFiber上调度
    Fiber(std::function<void()> cb, size_t stacksize = 0, bool use_caller=false);
    ~Fiber();
    
    //重置执行函数，并设置状态为INIT
    void reset(std::function<void()> cb);

    //切换到运行态
    void swapIn();

    //切换到后台
    void swapOut();


    //将当前协程切换到执行状态
    void call();


    //将当前协程切换到后台
    void back();


    uint64_t getId()const {return m_id;}

    State getState() const {return m_state;}


public:
    // 设置当前线程的运行协程
    static void SetThis(Fiber* f);


    //返回当前的协程
    static Fiber::ptr GetThis();

    //将当前协程切换到后台 并设置为READY状态
    static void YieldToReady();

    //返回当前的协程总数
    static uint64_t TotalFibers();

    //协程执行函数   执行完成后返回到线程主协程
    static void MainFunc();


    //协程执行函数   执行完成返回到线程调度协程
    static void CallerMainFunc();

    //返回当前协程ID
    static uint64_t GetFiberId();

    

private:
    uint64_t m_id = 0;
    uint32_t m_stacksize = 0;
    State m_state = INIT;
    ucontext_t m_ctx;
    void * m_stack = nullptr;
    std::function<void()> m_cb;


};

    



}


#endif
