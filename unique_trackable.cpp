#include <iostream>
#include <vector>

#include "unique_trackable.h"

struct Data{

};


int main() {
    using T = Data;
    std::vector<unique_trackable<T>> vec;
    vec.emplace_back();


    std::vector<unique_trackable<T>> vec2;

    copy(vec2, vec);

//    auto vec2 = std::move(vec);

    return 0;
}