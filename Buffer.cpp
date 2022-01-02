#include "Buffer.hpp"
#include<iostream>

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

void Buffer::put(int number)
{
    // sem_wait(&empty);
    wait(empty);
    enter();
    // std::scoped_lock lock(mutex);
    queue.push(number);
    leave();
    signal(full);
    // sem_post(&full);
}

int Buffer::get()
{
    int number;
    wait(full);
    // sem_wait(&full);
    enter();
    // std::scoped_lock lock(mutex);
    number = queue.front();
    queue.pop();
    leave();
    signal(empty);
    // sem_post(&empty);
    return number;
}