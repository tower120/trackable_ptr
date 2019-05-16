#include <iostream>

#include "test_utils.h"
#include "test_struct.h"

#include <tower120/trackable_ptr.h>

using namespace tower120;

struct Data : trackable_base {
    int i;
    explicit Data(int i) : i(i){}
};

int main(){
#if __cpp_deduction_guides
    {
        Data d{1};
        trackable_ptr p {&d};
        REQUIRE(std::is_same_v<decltype(p), trackable_ptr<Data>>);
    }
    {
        trackable<test_struct> d;
        trackable_ptr p {&d};
        REQUIRE(std::is_same_v<decltype(p), trackable_ptr<test_struct>>);
    }
    {
        const Data d{1};
        trackable_ptr p {&d};
        REQUIRE(std::is_same_v<decltype(p), trackable_ptr<const Data>>);
    }
    {
        const trackable<test_struct> d;
        trackable_ptr p {&d};
        REQUIRE(std::is_same_v<decltype(p), trackable_ptr<const test_struct>>);
    }
#endif
    return 0;
}