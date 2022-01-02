#include<queue>
#include<iostream>
#include"Buffer.hpp"
#include<array>
#include<chrono>
#include<thread>
#include<pthread.h>
#include<string>

std::size_t N = 1;
int MS = 1000;
Buffer buffer1{N};
Buffer buffer2{N};
Buffer buffer3{N};


void* producer1(void* arg){
    while(true){
    buffer2.put(1);
    std::string prompt2 = "Producer 1 inserted 1 into buffer 2\n";
    std::cout<<prompt2<<std::flush;
    std::this_thread::sleep_for(std::chrono::milliseconds(MS));
    buffer1.put(1);
    std::string prompt1 = "Producer 1 inserted 1 into buffer 1\n";
    std::cout<<prompt1<<std::flush;
    std::this_thread::sleep_for(std::chrono::milliseconds(MS));
    }
}

void* producer2(void* arg){
    while(true){
    buffer2.put(2);
    std::string prompt2 = "Producer 2 inserted 2 into buffer 2\n";
    std::cout<<prompt2<<std::flush;
    std::this_thread::sleep_for(std::chrono::milliseconds(MS));
    buffer3.put(2);
    std::string prompt1 = "Producer 2 inserted 2 into buffer 3\n";
    std::cout<<prompt1<<std::flush;
    std::this_thread::sleep_for(std::chrono::milliseconds(MS));
    }
}

void* consumer1(void* arg){
    while(true){
        int number = buffer1.get();
        std::string prompt = "Consumer 1 took " + std::to_string(number) + " from buffer 1\n";
        std::cout<< prompt <<std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(MS));
    }
}

void* consumer2(void* arg){
    while(true){
        int number = buffer2.get();
        std::string prompt = "Consumer 2 took " + std::to_string(number) + " from buffer 2\n";
        std::cout<< prompt <<std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(MS));
    }
}

void* consumer3(void* arg){
    while(true){
        int number = buffer3.get();
        std::string prompt = "Consumer 3 took " + std::to_string(number) + " from buffer 3\n";
        std::cout<< prompt <<std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(MS));
    }
}

int main()
{
    pthread_t prod1, prod2, cons1, cons2, cons3;
    pthread_create(&prod1, NULL, producer1, NULL);
    // pthread_create(&prod2, NULL, producer2, NULL);
    pthread_create(&cons1, NULL, consumer1, NULL);
    pthread_create(&cons2, NULL, consumer2, NULL);
    pthread_create(&cons3, NULL, consumer3, NULL);
    pthread_join(prod1, NULL);
    pthread_join(prod2, NULL);
    pthread_join(cons1, NULL);
    pthread_join(cons2, NULL);
    pthread_join(cons3, NULL);
    // pthread_exit(NULL);
    return 0;
}