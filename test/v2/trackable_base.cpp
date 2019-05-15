#include <iostream>

#include "../test_utils.h"

#include <tower120/v2/trackable_base.h>
#include <tower120/v2/trackable_ptr.h>

using namespace tower120::v2;

struct Box;

struct Corner{
    trackable_ptr<Box> box;
    int x = 0;
    int y = 0;
};

struct Box : trackable_base {
    Corner lt{this};
    Corner rb{this};
};

int main(){
    auto assert_box = [](Box& box){
        REQUIRE(box.lt.box == &box);
        REQUIRE(box.rb.box == &box);
    };

    Box box;
    trackable_ptr<Box> p = &box;
    assert_box(box);

    Box box2 = std::move(box);
    assert_box(box2);

    REQUIRE(p.get() == &box2);

    return 0;
}