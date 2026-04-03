#ifndef _THREAD_POOL_MANAGER_H
#define _THREAD_POOL_MANAGER_H
#include "ThreadPool.h"
#include <memory>

class ThreadPoolManager{
    public:
    static ThreadPoolManager& getInstance();

    ThreadPool& getNetworkPool();//网络I/O线程池
    ThreadPool& getGameLogicPool();//游戏逻辑线程池
    ThreadPool& getDataPool();//数据I/O线程池

private:
    ThreadPoolManager();
    ~ThreadPoolManager()=default;
    //删除拷贝构造函数和赋值操作符
    ThreadPoolManager(const ThreadPoolManager&)=delete;
    ThreadPoolManager& operator=(const ThreadPoolManager&) =delete;

    //线程池实例
    std::unique_ptr<ThreadPool> network_pool_;
    std::unique_ptr<ThreadPool> game_logic_pool_;
    std::unique_ptr<ThreadPool> data_pool_;
};

#endif