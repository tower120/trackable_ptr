#include <iostream>
#include <vector>

#include "trackable_ptr2.h"

template<class T>
using TrackablePtr = Tracker<T>;


struct Data{
    int x = 1;

    Data(){}
    Data(int x):x(x){}

    virtual void show(){
        std::cout << x << std::endl;
    }
};

void test(){
    Trackable<Data> data(20);

    Trackable<Data> data_copy = data;
    TrackablePtr<Data> p_data(data);


    Trackable<Data> data2{std::move(data)};
    data2.x = 2;

    TrackablePtr<Data> p_data2{std::move(p_data)};

    TrackablePtr<Data> p_data3{p_data2};
    // now p_data is null
    std::cout << p_data2->x << std::endl;
    std::cout << p_data3->x << std::endl;
    p_data3.get()->show();
}

int main() {
    // motivation example
    std::vector< Trackable<Data> > list1;

    auto makeData = [&]() -> TrackablePtr<Data>{
        list1.emplace_back();
        return  {list1.back()};
    };

    auto data = makeData();
    data->x = 100;

    std::cout << data.get() << std::endl;

    list1.emplace_back();
    list1.emplace_back();
    list1.emplace_back();
    list1.emplace_back();
    list1.emplace_back();

    // now data points to another location. (vector did realloc)
    std::cout << data.get() << std::endl;


    data->show();


    return 0;
}