#include <iostream>
#include <vector>
#include <chrono>
#include <memory>

#include <tower120/trackable_ptr.h>

template<class TimeUnit = std::chrono::milliseconds, class Closure>
static auto measure(Closure&& closure){
    using namespace std::chrono;
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
        closure();
    high_resolution_clock::time_point t2 = high_resolution_clock::now();

    return duration_cast<TimeUnit>(t2-t1).count();
}

template<class TimeUnit = std::chrono::milliseconds, class Closure>
static auto benchmark(unsigned int times, Closure&& closure){
	return measure<TimeUnit>([&]() {
	   for (unsigned int i = 0; i < times; ++i) closure();
	});
}

using namespace tower120;


namespace trackable{
    struct Box : trackable_base {
        Box(int x, int y) : x(x), y(y) {}

        int x;
        int y;

        int order = 0;
    };
}

namespace basic{
    struct Box {
        int x;
        int y;

        int order = 0;
    };
}

void read_benchmark(){
    const int size = 1'000'000;
    const int times = 100;

    {
        using namespace ::trackable;
        std::vector<Box> vec;
        std::vector<trackable_ptr<Box>> ptrs;

        for(int i=0;i<size ;i++){
            vec.emplace_back(i, i);
            ptrs.emplace_back(&vec.back());
        }
        int sum = 0;
        auto t = benchmark(times, [&](){
            for(const auto& e : ptrs){
                sum += e->x + e->y;
            }
        });

        std::cout << sum << " trackable " << t << std::endl;
    }
    {
        using namespace ::basic;
        std::vector<std::unique_ptr<Box>> vec;
        std::vector<Box*> ptrs;

        for(int i=0;i<size ;i++){
            vec.emplace_back(new Box{i, i});
            ptrs.emplace_back(vec.back().get());
        }
        int sum = 0;
        auto t = benchmark(times, [&](){
            for(const auto& e : ptrs){
                sum += e->x + e->y;
            }
        });

        std::cout << sum << " unique_ptr " << t << std::endl;
    }
}


int main() {
    read_benchmark();
    return 0;
}