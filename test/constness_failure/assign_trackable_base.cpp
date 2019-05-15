#include <iostream>

#include "../test_utils.h"
#include "../test_struct.h"

#include <tower120/trackable.h>
#include <tower120/trackable_ptr.h>

using namespace tower120::v2;

struct Data : trackable_base {
    int i;
    explicit Data(int i) : i(i){}
};

int main(){
    const Data d1{1};
    trackable_ptr<Data> p1;
    p1 = &d1;
    return 0;
}