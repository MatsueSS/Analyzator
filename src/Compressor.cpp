#include "Compressor.h"

#include <vector>
#include <chrono>

Compressor::Compressor() : sz(0)
{
    transfer.reserve(MAX_LOG_READ);
    compressed_info.reserve(MAX_SIZE_CODE);
}

Compressor::Compressor(Compressor&& obj) noexcept 
    : transfer(std::move(obj.transfer)), compressed_info(std::move(obj.compressed_info)), sz(obj.sz)
{
    obj.sz = 0;
}

Compressor& Compressor::operator=(Compressor&& obj) noexcept
{
    if(this == &obj)
        return *this;
    
    transfer = std::move(obj.transfer);
    compressed_info = std::move(obj.compressed_info);
    sz = obj.sz;
    obj.sz = 0;
    return *this;
}

void Compressor::insert(const std::pair<Code_value, Info_value>& obj)
{
    transfer.insert(obj);
    sz++;
}

void Compressor::insert(const Code_value& code, const Info_value& info)
{
    transfer.insert(std::make_pair(code, info));
    sz++;
}

int Compressor::size() const
{
    return sz;
}

void Compressor::handle_time()
{
    int max_time = 0;
    int count = 0;
    int average_time = 0;

    std::unordered_multimap<std::string, long long int> conn_map; //addr -> time
    std::vector<std::unordered_multimap<Code_value, Info_value>::iterator> item_erase;

    for(auto it = transfer.equal_range(100002); it.first != it.second; ++it.first){
        conn_map.insert(std::make_pair(it.first->second.get_addr(), std::stoll(it.first->second.get_time_t())));
        item_erase.push_back(it.first);
    }

    for(auto it = transfer.equal_range(100001); it.first != it.second; ++it.first){
        auto range = conn_map.equal_range(it.first->second.get_addr());

        for(auto conn_it = range.first; conn_it != range.second; conn_it++){
            int s = conn_it->second - std::stoll(it.first->second.get_time_t());
            average_time += s;
            max_time = max_time < s ? s : max_time;
            count++;
            item_erase.push_back(it.first);
            break;
        }
    }
    if(count != 0)
        average_time /= count;

    for(auto it : item_erase){
        transfer.erase(it);
        sz--;
    }

    compressed_info[400004] += count;
    compressed_info[400001] = compressed_info[400001] > max_time ? compressed_info[400001] : max_time;
    compressed_info[400002] = compressed_info[400002] > average_time ? compressed_info[400002] : average_time;
}

void Compressor::make_compress()
{
    const static std::vector<Code_value> count_codes = {
        107, 402, 1001, 1003, 2001, 200001
    };
    int count;

    auto it_vec = count_codes.begin();
    while(it_vec != count_codes.end()){
        count = 0;
        auto it_transf = transfer.equal_range(*it_vec);
        for(auto it = it_transf.first; it != it_transf.second; it++){
            count++;
        }
        if(count == 0){
            it_vec++;
            continue;
        }
        sz -= count;
        compressed_info[*it_vec] += count;
        transfer.erase(*it_vec);
        it_vec++;
    }

    if(sz == 0)
        return;

    handle_time();

    transfer.clear();
    sz = 0;
}