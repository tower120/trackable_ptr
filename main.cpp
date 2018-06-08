#include <iostream>
#include <vector>

#include "trackable_ptr.h"

struct Data{
    int x,y;
};


int main() {
    using namespace tower120::utils;

    std::vector< unique_trackable<Data> > vec;
    trackable_ptr<Data> ptr1 = &vec.emplace_back(Data{10,20});
    vec.emplace_back(Data{1,2});
    vec.emplace_back(Data{1,2});
    vec.emplace_back(Data{1,2});
    vec.emplace_back(Data{1,2});

    for(auto& data : vec){
        std::cout << data->x << " : " << data->y << std::endl;
    }


    auto ptr = std::move(ptr1);

    std::cout << ptr->x << " : " << ptr->y << std::endl;


    return 0;
}