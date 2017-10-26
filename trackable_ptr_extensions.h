#ifndef TRACKABLE_PTR_TRACKABLE_PTR_EXTENSIONS_H
#define TRACKABLE_PTR_TRACKABLE_PTR_EXTENSIONS_H

#include <vector>
#include "trackable_ptr.h"

template<class T, class R, class ...VecAdditionals>
static std::size_t get_index(const std::vector<T, VecAdditionals...>& vec, const trackable_ptr<R>& ptr){
    return ptr.get() - vec.data();
}

template<class T, class R, class ...VecAdditionals>
static auto get_iterator(const std::vector<T, VecAdditionals...>& vec, const trackable_ptr<R>& ptr){
    return vec.begin() + get_index(vec, ptr);
}


#endif //TRACKABLE_PTR_TRACKABLE_PTR_EXTENSIONS_H
