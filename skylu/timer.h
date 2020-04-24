/*************************************************************************
	> File Name: timer.h
	> Author: 
	> Mail: 
	> Created Time: 2020年04月22日 星期三 07时45分07秒
 ************************************************************************/

#ifndef _TIMER_H
#define _TIMER_H
#include <memory>
#include <vector>
#include <set>
#include "thread.h"

namespace skylu{
class TimerManager;

class Timer: public std::enable_shared_from_this<Timer>
{
    friend class TimerManager;
public:
    typedef  std::shared_ptr<Timer>  ptr;

    /* 
     *
     * @brief 取消定时器
     *
     */
    bool cancel();

    /*
     *
     * @brief 刷新设置定时器的执行时间
     *
     */
    bool refresh();

    /*
     *
     *@brief 重置定时器时间
     *@param[in] ms 定时器执行间隔时间
     *@param[in] from_now 是否从当前时间开始计算
     */
    bool reset(uint64_t  ms,bool from_now);

private:


    Timer(uint64_t ms,std::function<void()> cb,
         bool isloop,TimerManager* manager);

    Timer(uint64_t next);

private:
    bool m_isloop = false;
    uint64_t m_ms = 0; //执行周期
    uint64_t m_next = 0; //精确的执行时间
    std::function<void()> m_cb;
    TimerManager* m_manager = nullptr;

private:
    /*
     *
     * @brief 比较定时器的智能指针大小（按照执行时间排序
     */
    struct Comparator{
        
        bool operator()(const Timer::ptr& lhs,const Timer::ptr& rhs) const;
    }
        
};


class TimerManager{
    friend class Timer;
public:
    typedef RWMutex RWMutexType;

    TimerManager();

    virtual ~TimerManager();

    Timer::ptr addTimer(uint64_t ms,std::function<void()> cb,
                       bool isloop = false);

    /*
     *
     *@brief 添加条件定时器
     *@param [in] weak_cond 条件
     */
    Timer::ptr addConditionTimer(uint64_t ms, std::function<void()> cb,
                                 std::weak_ptr<void> weak_cond,
                                bool isloop = false);

    /*
     *
     * @brief 到最近一个定时器执行的时间间隔
     *
     */
    uint64_t getNextTimer();

    /*
     *
     * @brief 获取需要执行的定时器的回调函数列表
     * @param[out] cbs 回调函数数组
     */
    void listExpiredCb(std::vector<std::function<void()> > & cbs);

    bool hasTimer();

protected:

    /*
     *
     * @brief 当有新的定时器插入到定时器的首部，执行该函数
     *
     */
    virtual void onTimerInsertedAtFront() = 0;
    

    void addTimer(Timer::ptr val,RWMutexType::WriteLock& lock);

private:
    /*
     *
     * @brief 检测服务器时间是否被调后
     *
     */
    bool detectClockRollover(uint64_t now_ms);


private:
    RWMutexType m_mutex;
    std::set<Timer::ptr,Timer::Comparator> m_timers;
    //是否触发onTimerInsertedAtFront
    bool m_tickled = false;
    //上次执行的时间
    uint64_t m_previouseTime = 0;

    


};

}
#endif
