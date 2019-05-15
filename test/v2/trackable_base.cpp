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
    return 0;
}