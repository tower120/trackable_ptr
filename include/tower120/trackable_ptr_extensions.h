#ifndef TRACKABLE_PTR_TRACKABLE_PTR_EXTENSIONS_H
#define TRACKABLE_PTR_TRACKABLE_PTR_EXTENSIONS_H

#include <vector>
#include "trackable_ptr.h"


namespace tower120 {

    template<class T, class Container>
    static std::size_t get_index(const Container &container, const trackable_ptr<T> &ptr) {
        return ptr.get_trackable() - container.data();
    }

    template<class T, class Container>
    static auto get_iterator(const Container &container, const trackable_ptr<T> &ptr) {
        return container.begin() + get_index(container, ptr);
    }

}


#endif //TRACKABLE_PTR_TRACKABLE_PTR_EXTENSIONS_H
