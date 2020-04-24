#include "mutex.h"
#include <stdexcept>
#include "scheduler.h"

namespace skylu{
Semaphore::Semaphore(uint32_t count){
    if(sem_init(&m_semaphore,0,count)){
        throw std::logic_error("sem_init error");
   }

}

Semaphore::~Semaphore()
{
    sem_destroy(&m_semaphore);

}


void Semaphore::wait()
{
    if(sem_wait(&m_semaphore)){
        throw std::logic_error("sem_wait error");
    }
        
}

void Semaphore::notify()
{
    if(sem_post(&m_semaphore)){
        throw std::logic_error("sem_post error");
    }

}

FiberSemaphore::FiberSemaphore(size_t initial_concurrency)
    :m_concurrency(initial_concurrency)
{
        
}

FiberSemaphore::~FiberSemaphore()
{

        
}


bool FiberSemaphore::tryWait()
{
    {
        MutexType::Lock lock(m_mutex);
        if(m_concurrency > 0)
        {
            --m_concurrency;
            return true;
        }
        return false;
    }

}


void FiberSemaphore::wait()
{
    {
        MutexType::Lock lock(m_mutex);
        if(m_concurrency > 0)
        {
            --m_concurrency;
            return ;
        }
        m_waitters.push_back(std::make_pair(Scheduler::GetThis(),Fiber::GetThis()));
    }
    Fiber::YieldToHold();

}

void FiberSemaphore::notify()
{
    MutexType::Lock lock(m_mutex);
    if(!m_waitters.empty()){
        auto next = m_waitters.front();
        m_waitters.pop_front();
        next.first->schedule(next.second);
    }else {
        ++m_concurrency;
    }
}
        



}



