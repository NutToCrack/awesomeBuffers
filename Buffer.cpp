#include "Buffer.hpp"
#include<iostream>
#include<iostream>
#include<thread>
#include<chrono>

int MS = 1000;


Buffer::Buffer(std::size_t size):
empty(size), full(0)
{
    // this->size = size;
    // sem_init(&empty, 0, size);
    // sem_init(&full, 0, 0);
}

Buffer::~Buffer()
{
    std::cout << "DESTRUCTOR CALLED" << std::endl;
    // sem_destroy(&empty);
    // sem_destroy(&full);
}

void Buffer::put(int number, std::string name)
{
    // enter();
    // sem_wait(&empty);
    enter();
    if (queue.size() == size) wait(empty);
    //{
    // std::scoped_lock lock(mutex);
    queue.push(number);
    //}
    std::cout<<name<<std::flush;
    std::this_thread::sleep_for(std::chrono::milliseconds(MS));
    // std::cout<< "QUEUEUUEUEUEUEU SIZE PUTTER" + std::to_string(queue.size())<<std::endl;
    signal(full);
    leave();
    // sem_post(&full);
    // leave();
}

int Buffer::get(std::string name, std::string vorname)
{
    enter();
    int number;
    if (queue.size() == 0) wait(full);
    // sem_wait(&full);
    // std::scoped_lock lock(mutex);
    number = queue.front();
    queue.pop();
    // std::string prompt = "Consumer 1 took " + std::to_string(number) + " from buffer 1\n";
    // std::cout<< prompt <<std::flush;
    std::cout<<name + " " + std::to_string(number) + vorname<<std::flush;
    std::this_thread::sleep_for(std::chrono::milliseconds(MS));
    // std::cout<< "QUEUEUUEUEUEUEU SIZE GETTTER" + std::to_string(queue.size())<<std::endl;
    // leave();
    signal(empty);
    // sem_post(&empty);
    leave();
    return number;
}