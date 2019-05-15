#include <iostream>

#include "test_utils.h"
#include "test_struct.h"

#include <tower120/trackable.h>
#include <tower120/trackable_ptr.h>

using namespace tower120;

template<class T>
constexpr const bool is_const =
    std::is_const<
        std::remove_reference_t<
            std::remove_pointer_t<T>
        >
    >::value;

struct Data : trackable_base {
    int i;
    explicit Data(int i) : i(i){}
};

void ctr_trackable(){
    {
        trackable<test_struct> d1{1};
        trackable_ptr<test_struct> p1 {&d1};
        REQUIRE(p1->i == 1);
        REQUIRE(!is_const<decltype(p1.get())>);
    }
    {
        trackable<test_struct> d1{{1}};
        trackable_ptr<const test_struct> p1 {&d1};
        REQUIRE(p1->i == 1);
        REQUIRE(is_const<decltype(p1.get())>);
    }
    {
        trackable<const test_struct> d1{{1}};
        trackable_ptr<const test_struct> p1 {&d1};
        REQUIRE(p1->i == 1);
        REQUIRE(is_const<decltype(d1.get())>);
        REQUIRE(is_const<decltype(p1.get())>);
    }
}

void ctr_trackable_base(){
    {
        Data d1{1};
        trackable_ptr<Data> p1 {&d1};
        REQUIRE(p1->i == 1);
        REQUIRE(!is_const<decltype(p1.get())>);
    }
    {
        Data d1{1};
        trackable_ptr<const Data> p1 {&d1};
        REQUIRE(p1->i == 1);
        REQUIRE(is_const<decltype(p1.get())>);
    }
    {
        const Data d1{1};
        trackable_ptr<const Data> p1 {&d1};
        REQUIRE(p1->i == 1);
        REQUIRE(is_const<decltype(d1)>);
        REQUIRE(is_const<decltype(p1.get())>);
    }
}

void assign_trackable(){
    {
        trackable<test_struct> d1{1};
        trackable_ptr<test_struct> p1;
        p1 = &d1;
        REQUIRE(p1->i == 1);
        REQUIRE(!is_const<decltype(p1.get())>);
    }
    {
        trackable<test_struct> d1{1};
        trackable_ptr<const test_struct> p1;
        p1 = &d1;
        REQUIRE(p1->i == 1);
        REQUIRE(is_const<decltype(p1.get())>);
    }
    {
        trackable<const test_struct> d1{1};
        trackable_ptr<const test_struct> p1;
        p1 = &d1;
        REQUIRE(p1->i == 1);
        REQUIRE(is_const<decltype(d1.get())>);
        REQUIRE(is_const<decltype(p1.get())>);
    }
}

void assign_trackable_base(){
    {
        Data d1{1};
        trackable_ptr<Data> p1;
        p1 = &d1;
        REQUIRE(p1->i == 1);
        REQUIRE(!is_const<decltype(p1.get())>);
    }
    {
        Data d1{1};
        trackable_ptr<const Data> p1;
        p1 = &d1;
        REQUIRE(p1->i == 1);
        REQUIRE(is_const<decltype(p1.get())>);
    }
    {
        const Data d1{1};
        trackable_ptr<const Data> p1;
        p1 = &d1;
        REQUIRE(p1->i == 1);
        REQUIRE(is_const<decltype(d1)>);
        REQUIRE(is_const<decltype(p1.get())>);
    }
}

int main(){
    ctr_trackable();
    ctr_trackable_base();
    assign_trackable();
    assign_trackable_base();
    return 0;
}