#include <iostream>

#include "test_utils.h"
#include "test_struct.h"

#include <tower120/trackable_ptr.h>
#include <tower120/trackable_ptr_extensions.h>

#include <vector>

using namespace tower120;

void test_trackable(){
    std::vector<trackable<test_struct>> vec;
    vec.emplace_back(1);
    trackable_ptr<test_struct> p{&vec.front()};
    REQUIRE(p->i == 1);

    for(int i=2;i<1000;++i){
        vec.emplace_back(i);
    }
    REQUIRE(p->i == 1);

    REQUIRE(p.get() == vec.front().get());
    std::iter_swap(get_iterator(vec, p), std::prev(vec.end()));
    REQUIRE(p.get() == vec.back().get());
    REQUIRE(p->i == 1);
}

void test_trackable_base(){
    struct I : trackable_base {
        int i;
        I(int i) : i(i) {}
    };

    std::vector<I> vec;
    vec.emplace_back(1);
    trackable_ptr<I> p{&vec.front()};
    REQUIRE(p->i == 1);

    for(int i=2;i<1000;++i){
        vec.emplace_back(i);
    }
    REQUIRE(p->i == 1);

    REQUIRE(p.get() == &vec.front());
    std::iter_swap(get_iterator(vec, p), std::prev(vec.end()));
    REQUIRE(p.get() == &vec.back());
    REQUIRE(p->i == 1);
}

int main(){
    test_trackable();
    test_trackable_base();
    return 0;
}