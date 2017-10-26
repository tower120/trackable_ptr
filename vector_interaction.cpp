#include <vector>
#include <iostream>

#include "trackable_ptr_extensions.h"

struct Data{
    int value;

    Data(int value)
        :value(value)
    {}
};

int main() {
    std::vector<trackable<Data>> vec;

    trackable<Data>& d1 = vec.emplace_back(1);
    trackable_ptr<Data> p_i1{d1};

    trackable<Data>& d2 = vec.emplace_back(2);
    trackable_ptr<Data> p_i2{d2};

    vec.erase(get_iterator(vec, p_i1));

    trackable_ptr<Data> p_i3 = *vec.insert(get_iterator(vec, p_i2), 3);

    std::cout << "dead " << !p_i1 << std::endl;
    std::cout << p_i2->value << std::endl;
    std::cout << p_i3->value << std::endl;

    return 0;
}