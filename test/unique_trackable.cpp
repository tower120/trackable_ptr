#include <iostream>

#include "test_utils.h"
#include "test_struct.h"

#include <tower120/trackable.h>
#include <tower120/trackable_ptr.h>

using namespace tower120;

// trackable move - transfer all it's trackable_ptr's

void move_ctr_test(){
    unique_trackable<test_struct> d2{{2}};
    trackable_ptr<test_struct> p2 {&d2};
    REQUIRE(p2->i == 2);

    unique_trackable<test_struct> d1{std::move(d2)};
    trackable_ptr<test_struct> p1 {&d1};
    REQUIRE(p1->i == 2);
    REQUIRE(p2->i == 2);

    REQUIRE(p1 == p2);

    REQUIRE(ptrs_count(d1) == 2);
    REQUIRE(ptrs_count(d2) == 0);
}

void move_test(){
    unique_trackable<test_struct> d1{{1}};
    unique_trackable<test_struct> d2{{2}};

    trackable_ptr<test_struct> p1 {&d1};
    trackable_ptr<test_struct> p2 {&d2};
    REQUIRE(p1->i == 1);
    REQUIRE(p2->i == 2);
    d1 = std::move(d2);
    REQUIRE(!p1);
    REQUIRE(p2->i == 2);

    REQUIRE(ptrs_count(d1) == 1);
    REQUIRE(ptrs_count(d2) == 0);
}

void death_test(){
    trackable_ptr<test_struct> p;
    {
        unique_trackable<test_struct> data{{1}};
        p = &data;
        REQUIRE(p->i == 1);
    }
    REQUIRE(!p);
}

int main(){
    move_ctr_test();
    move_test();
    death_test();
    return 0;
}