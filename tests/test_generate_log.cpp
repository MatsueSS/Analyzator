#include "Log.h"

#include <iostream>
#include <chrono>
#include <thread>

void thread_work1()
{
    while(true){
        std::this_thread::sleep_for(std::chrono::seconds(1));
        Log::make_note("1001");
    }
}

void thread_work2()
{
    while(true){
        std::this_thread::sleep_for(std::chrono::seconds(2));
        Log::make_note("1003");
    }
}

void thread_work3()
{
    while(true){
        std::this_thread::sleep_for(std::chrono::seconds(5));
        Log::make_note("2001");
    }
}

void thread_work4()
{
    while(true){
        std::this_thread::sleep_for(std::chrono::seconds(1));
        Log::make_note("200001");
    }
}

int main(void)
{
    Log::make_note("107");
    
    std::thread th1(thread_work1);
    std::thread th2(thread_work2);
    std::thread th3(thread_work3);
    std::thread th4(thread_work4);

    th1.join();
    th2.join();
    th3.join();
    th4.join();
}