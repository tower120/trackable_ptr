#include <iostream>

#include "../test_utils.h"
#include "../test_struct.h"

#include <tower120/trackable.h>
#include <tower120/trackable_ptr.h>

using namespace tower120;

int main(){
    trackable<const test_struct> d1{1};
    trackable_ptr<test_struct> p1;
    p1 = &d1;
    return 0;
}