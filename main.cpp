#include <iostream>
#include <vector>

#include "trackable_ptr.h"

template<class T>
using TrackablePtr = trackable_ptr<T>;


struct IData{
    int x = 1;

    IData(){}
    IData(int x):x(x){}

    virtual void show() = 0;
};

struct Data : IData{
    using IData::IData;
    virtual void show() override {
        std::cout << x << std::endl;
    }
};

struct Data2 : IData{
    using IData::IData;
    virtual void show() override {
        std::cout << "Data 2 " << x << std::endl;
    }
};


void test_TrackerBase(){
    struct IData : trackable_base{
        int x = 1;

        IData(){}
        IData(int x):x(x){}

        virtual void show() = 0;
    };

    struct Data : IData{
        using IData::IData;
        virtual void show() override {
            std::cout << x << std::endl;
        }
    };

    struct Data2 : IData{
        using IData::IData;
        virtual void show() override {
            std::cout << "Data 2 " << x << std::endl;
        }
    };

    Data data(20);

    trackable_ptr<IData> p_data(data);
    p_data->show();

    trackable_ptr<IData> p_data_moved(std::move(p_data));

    p_data_moved = p_data;
}


void test_virtual(){
    trackable<Data> data(20);

    trackable_ptr<IData> p_data(data);
    p_data->show();

    trackable<Data2> data2(100);
    p_data = {data2};
    p_data->show();


    p_data = {data};
    std::cout << p_data.get() << std::endl;

    Data data_moved{std::move(data)};
    data.x = 40;

    trackable<IData>* ptr =p_data.get();
    std::cout <<  ptr<< std::endl;
    p_data->show();
}


int main() {
    test_TrackerBase();
    test_virtual();
    /*// motivation example
    std::vector< trackable<Data> > list1;

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

*/
    return 0;
}