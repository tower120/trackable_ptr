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
    struct Box;

    struct Corner{
        explicit Corner(Box* box) : box(box) {}
        trackable_ptr<Box> box;
        int i;
    };

    struct Box : trackable_base {
        Corner lt{this};
        Corner rb{this};

        int sum() const {
            return lt.i + rb.i;
        }

        explicit Box(int i) {
            lt.i = i;
            rb.i = i;
        }
    };
}

namespace basic{
    struct Box;

    struct Corner{
        explicit Corner(Box* box) : box(box) {}
        Box* box;
        int i;
    };

    struct Box {
        Corner lt{this};
        Corner rb{this};

        int sum() const {
            return lt.i + rb.i;
        }

        explicit Box(int i) {
            lt.i = i;
            rb.i = i;
        }
    };
}

void fill_benchmark(){
    const int size = 1000;
    const int times = 100'0;

    {
        auto t = benchmark(times, [&](){
            std::vector<::trackable::Box> vec;
            for(int i=0;i<size ;i++){
                vec.emplace_back(i);
            }
        });

        std::cout << "trackable " << t << std::endl;
    }

    {
        auto t = benchmark(times, [&](){
            std::vector<::basic::Box> vec;
            for(int i=0;i<size ;i++){
                vec.emplace_back(i);
            }
        });

        std::cout << "basic " << t << std::endl;
    }

    {
        auto t = benchmark(times, [&](){
            std::vector<std::unique_ptr<::basic::Box>> vec;
            for(int i=0;i<size ;i++){
                vec.emplace_back(std::make_unique<::basic::Box>(i));
            }
        });

        std::cout << "unique_ptr " << t << std::endl;
    }
}

void read_benchmark(){
    const int size = 1'000'000;
    const int times = 100;

    {
        std::vector<::trackable::Box> vec;
        for(int i=0;i<size ;i++){
            vec.emplace_back(i);
        }
        int sum = 0;
        auto t = benchmark(times, [&](){
            for(auto& e : vec){
                sum += e.sum();
            }
        });

        std::cout << sum << " trackable " << t << std::endl;
    }

    {
        std::vector<::basic::Box> vec;
        for(int i=0;i<size ;i++){
            vec.emplace_back(i);
        }
        int sum = 0;
        auto t = benchmark(times, [&](){
            for(auto& e : vec){
                sum += e.sum();
            }
        });

        std::cout << sum << " basic " << t << std::endl;
    }

    {
        std::vector<std::unique_ptr<::basic::Box>> vec;
        for(int i=0;i<size ;i++){
            vec.emplace_back(std::make_unique<::basic::Box>(i));
        }
        int sum = 0;
        auto t = benchmark(times, [&](){
            for(auto& e : vec){
                sum += e->sum();
            }
        });

        std::cout << sum << " unique_ptr " << t << std::endl;
    }

}

int main() {
    std::cout << "fill test" << std::endl;
    fill_benchmark();
    std::cout << "read test" << std::endl;
    read_benchmark();
    return 0;
}