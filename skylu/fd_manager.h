/*************************************************************************
	> File Name: fd_manager.h
	> Author: 
	> Mail: 
	> Created Time: 2020年04月24日 星期五 10时40分24秒
    文件句柄管理
 ************************************************************************/

#ifndef _FD_MANAGER_H
#define _FD_MANAGER_H
#include <memory>
#include <vector>
#include "thread.h"
#include <singleton.h>

namespace skylu{

class FdCtx : public std::enable_shared_from_this<FdCtx>{
public:
    typedef std::shared_ptr<FdCtx> ptr;

    FdCtx(int fd);

    ~FdCtx();

    bool isInit() const {return m_isInit;}

    bool isSocket()const {return m_isSocket;}

    bool isClose() const {returm m_isClosed;}

    void setUserNonblock(bool v){m_userNonblock = v;}

    bool getUserNonblock() const {return m_userNonblock;}

    void setSysNonblock(bool v){m_sysNonblock = v;}

    bool getSysNonblock() const {return m_sysNonblock;}

    void setTimeout(int type,uint64_t v);

    uint64_t getTimeout(int type);
        

private:
    bool init();


private:
    bool m_isInit = true;
    bool m_isSocket = true;

    bool m_sysNonblock = true;
    bool m_userNonblock = true;

    bool m_isClosed = true;
    bool m_fd;

    uint64_t m_recvTimeout;
    uint64_t m_sendTimeout;

};


    //文件句柄管理类
class FdManager{

public:
    typedef RWMutex RWMutexType;

    FdManager();


    /*
     * @brief 获取/创建文件句柄类 
     * @param[in] Fd 
     * @param[in] auto_create: 是否自动创建
     *
     */
    FdCtx::ptr get(int fd,bool auto_create = false);

    void del(int fd);
private:
    RWMutexType m_mutex;
    std::vector<FdCtx::ptr> m_datas;

        
};

// 单例类
typedef singleton<FdManager> FdMgr;
}
#endif


