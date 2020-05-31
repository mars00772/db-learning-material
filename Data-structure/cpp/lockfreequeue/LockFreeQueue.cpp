//
// Created by AlexanderChiu on 2020/5/31.
//

#include "LockFreeQueue.h"
#include <time.h>
#include <mutex>
#include <iostream>
#include <queue>

int main(){

    //测试无锁队列
    LockFreeQueue<int>lfq;
    std::queue<int>q;
    std::mutex mutex;

    {
        size_t costTime = 0;
        std::vector<std::thread> threads;
        for(int i = 0; i < 5; i++){
            threads.emplace_back(std::thread([&]{
                size_t beginTime = clock();
                for(int j = 0; j < 1000000;j++){
                    lfq.enqueue(j*j);
                }
                for(int j = 0; j < 1000000;j++){
                    lfq.dequeue();
                }
                size_t endTime = clock();
                costTime+=endTime-beginTime;
            }));
        }
        for(auto& t : threads){
            t.join();
        }
        std::cout<<costTime<<std::endl;
    }


    {
        size_t costTime = 0;
        std::vector<std::thread> threads;
        for(int i = 0; i < 5; i++){
            threads.emplace_back(std::thread([&]{
                size_t beginTime = clock();
                for(int j = 0; j < 1000000;j++){
                    mutex.lock();
                    q.push(j*j);
                    mutex.unlock();
                }
                for(int j = 0; j< 1000000;j++){
                    mutex.lock();
                    q.pop();
                    mutex.unlock();
                }
                size_t endTime = clock();
                costTime+=endTime-beginTime;
            }));
        }
        for(auto& t : threads){
            t.join();
        }
        std::cout<<costTime<<std::endl;
    }
}