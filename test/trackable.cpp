#include <iostream>

#include "test_utils.h"
#include "test_struct.h"

#include <tower120/trackable.h>
#include <tower120/trackable_ptr.h>

using namespace tower120;

// trackable copy - only copy value

void copy_ctr_test(){
    trackable<test_struct> d2{{2}};
    trackable_ptr<test_struct> p2 {&d2};
    REQUIRE(p2->i == 2);

    trackable<test_struct> d1{d2};
    trackable_ptr<test_struct> p1 {&d1};
    REQUIRE(p1->i == 2);
    REQUIRE(p2->i == 2);

    REQUIRE(p1 != p2);

    REQUIRE(ptrs_count(d1) == 1);
    REQUIRE(ptrs_count(d2) == 1);
}

void copy_test(){
    trackable<test_struct> d1{{1}};
    trackable<test_struct> d2{{2}};

    trackable_ptr<test_struct> p1 {&d1};
    trackable_ptr<test_struct> p2 {&d2};
    REQUIRE(p1->i == 1);
    REQUIRE(p2->i == 2);
    d1 = d2;
    REQUIRE(p1->i == 2);
    REQUIRE(p2->i == 2);

    REQUIRE(p1 != p2);

    REQUIRE(ptrs_count(d1) == 1);
    REQUIRE(ptrs_count(d2) == 1);
}

int main(){
    copy_ctr_test();
    copy_test();
    return 0;
}