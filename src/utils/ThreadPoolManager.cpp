#include "ThreadPoolManager.h"
#include <thread>
#include <mutex>


ThreadPoolManager& ThreadPoolManager::getInstance(){
    static ThreadPoolManager instance;
    return instance;
}

ThreadPoolManager::ThreadPoolManager(){
    //获取CPU核心
    unsigned int num_threads=std::thread::hardware_concurrency();
    if(num_threads==0)
        num_threads=4;
    //网络线程池
    unsigned int network_threads=std::min(8u,std::max(4u,num_threads/2));
    network_pool_=std::make_unique<ThreadPool>(network_threads);

    //逻辑池
    game_logic_pool_=std::make_unique<ThreadPool>(num_threads);
    //数据池子
    data_pool_=std::make_unique<ThreadPool>(std::min(4u,num_threads/2));
}
ThreadPool& ThreadPoolManager::getNetworkPool(){
    return *network_pool_;
}

ThreadPool& ThreadPoolManager::getGameLogicPool(){
    return *game_logic_pool_;
}
ThreadPool& ThreadPoolManager::getDataPool(){
    return *data_pool_;
}