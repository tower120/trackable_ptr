#pragma once

#include <vector>
#include "trackable_ptr.h"

namespace tower120 {

    // TODO: use ContiguousIterator concept in cpp20

    template<class T, class Container>
    bool in_container(const Container& container, const trackable_ptr<T>& ptr) {
        return
            ptr
            && !container.empty()
            && ptr.get_trackable() >= &container.front() && ptr.get_trackable() <= &container.back();
    }

    template<class T, class Container>
    std::size_t get_index(const Container& container, const trackable_ptr<T>& ptr) {
        assert(in_container(container, ptr));
        return ptr.get_trackable() - container.data();
    }

    /// Container must be bounded (Iterator == Sentinel)
    /// return constainer.end() if !ptr
    template<class T, class Container>
    decltype(auto) get_iterator(Container&& container, const trackable_ptr<T>& ptr) {
        if (!ptr) return container.end();
        return container.begin() + get_index(container, ptr);
    }

}