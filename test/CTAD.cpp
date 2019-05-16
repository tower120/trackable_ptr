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
        REQUIRE(std::is_same<decltype(p), trackable_ptr<Data>>::value);
    }
    {
        trackable<test_struct> d;
        trackable_ptr p {&d};
        REQUIRE(std::is_same<decltype(p), trackable_ptr<test_struct>>::value);
    }
    {
        const Data d{1};
        trackable_ptr p {&d};
        REQUIRE(std::is_same<decltype(p), trackable_ptr<const Data>>::value);
    }
    {
        const trackable<test_struct> d;
        trackable_ptr p {&d};
        REQUIRE(std::is_same<decltype(p), trackable_ptr<const test_struct>>::value);
    }
#endif
    return 0;
}