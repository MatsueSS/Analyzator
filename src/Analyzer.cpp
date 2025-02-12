#include "Analyzer.h"
#include "Result_generator.h"
#include "TCP_server.h"
#include "Log.h"

Analyzer::Analyzer() {}

void Analyzer::time_check()
{
    auto t = std::chrono::steady_clock::now();
    while(true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(3600));
        if(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now()
        - t).count() > 3600*24){
            t = std::chrono::steady_clock::now();
            std::unique_ptr<Compressor> compressor = std::make_unique<Compressor>();
            std::unique_ptr<CSV_reader> csv_reader = std::make_unique<CSV_reader>("../res/codes.csv");
            Log::read_all_note(compressor);
            Result_generator result(std::move(compressor), std::move(csv_reader));
            result.generate_report();
        }
    }
}

void Analyzer::start_process()
{
    std::thread time_checker_thread(&Analyzer::time_check, this);

    TCP_server server;
    server.socket();
    server.bind();
    server.listen();
    server.start();
}