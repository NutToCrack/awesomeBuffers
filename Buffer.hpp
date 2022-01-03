#ifndef BUFFER
#define BUFFER

#include<queue>
// #include<semaphore.h>
// #include<mutex>
#include "monitor.h"


class Buffer: public Monitor
{
private:
    std::size_t size;
    std::queue<int> queue;
    // sem_t full;
    // sem_t empty;
    // std::mutex mutex;
    Condition full;
    Condition empty;
public:
    Buffer(std::size_t size);
    ~Buffer();
    void put(int number, std::string name);
    int get(std::string name, std::string vorname);
};

#endif