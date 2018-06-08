#ifndef TRACKABLE_PTR_UNIQUE_TRACKABLE_H
#define TRACKABLE_PTR_UNIQUE_TRACKABLE_H

#include <vector>

#include "trackable_ptr.h"

template<class T>
class unique_trackable : public trackable<T>{
    using Base = trackable<T>;

public:
    unique_trackable(const unique_trackable&) = delete;

    using Base::Base;

    unique_trackable(const Base& other_trackable)
        : Base(*static_cast<const Base*>(&other_trackable))
    {}

    unique_trackable(unique_trackable&&) noexcept {}
    ~unique_trackable() noexcept {}
};

template<class T>
void copy(std::vector<unique_trackable<T>>& to, const std::vector<unique_trackable<T>>& from){
    to.clear();
    for(auto& element : from){
        to.emplace_back(*static_cast<const trackable<T>*>(&element));
    }
}

#endif //TRACKABLE_PTR_UNIQUE_TRACKABLE_H
