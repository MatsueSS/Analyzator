#include "Log.h"
#include "Result_generator.h"

#include <iostream>
#include <chrono>
#include <thread>

void thread_work1()
{
    while(true){
        std::this_thread::sleep_for(std::chrono::seconds(2));
        Log::make_note("1001");
    }
}

void thread_work2()
{
    while(true){
        std::this_thread::sleep_for(std::chrono::seconds(3));
        Log::make_note("1003");
    }
}

void thread_work3()
{
    while(true){
        std::this_thread::sleep_for(std::chrono::seconds(4));
        Log::make_note("2001");
    }
}

void thread_work4()
{
    while(true){
        std::this_thread::sleep_for(std::chrono::seconds(7));
        Log::make_note("200001");
    }
}

void fone_work()
{
    auto t = std::chrono::steady_clock::now();
    while(true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(10));
        if(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now()
        - t).count() > 300){
            t = std::chrono::steady_clock::now();
            std::unique_ptr<Compressor> compressor = std::make_unique<Compressor>();
            std::unique_ptr<CSV_reader> csv_reader = std::make_unique<CSV_reader>("../res/codes.csv");
            Log::read_all_note(compressor);
            Result_generator result(std::move(compressor), std::move(csv_reader));
            result.generate_report();
        }
    }

}

int main(void)
{
    Log::new_log_filename();
    Log::make_note("107");
    
    std::thread th1(thread_work1);
    std::thread th2(thread_work2);
    std::thread th3(thread_work3);
    std::thread th4(thread_work4);
    std::thread th5(fone_work);

    th1.join();
    th2.join();
    th3.join();
    th4.join();
    th5.join();
}