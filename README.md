# trackable_ptr

Trackable pointer. When trackable object moved/destroyed, trackers updated with new object's pointer.

Allow to have stable pointer on any movable object (in single-threaded environment). Objects may be stack allocated.

[Live example](http://coliru.stacked-crooked.com/a/c6a2e71ea86f8902)

```c++
struct Data{
    int x,y,z;
};

std::vector< unique_trackable<Data> > list1;

list1.emplace_back();

	trackable_ptr<Data> data {list1.back()};	//  store pointer to element

list1.emplace_back();
list1.emplace_back();
list1.emplace_back();
list1.emplace_back();
// list 1 now uses another memory chuck. All pointers/iterators invalidated.

// data still alive and accessible;
std::cout << data->x;
```


Dead elements have nullptr `trackable_ptr`'s.



#### Interaction with `std::vector`

```c++
#include <vector>
#include <iostream>

#include "trackable_ptr_extensions.h"

struct Data{
    int value;
    Data(int value) :value(value) {}
};

int main() {
    std::vector<trackable<Data>> vec;

    // fill
    trackable<Data>& d1 = vec.emplace_back(1);
    trackable_ptr<Data> p_i1{d1};

    trackable_ptr<Data> p_i2{vec.emplace_back(2)};

    // erase
    vec.erase(get_iterator(vec, p_i1));

    // insert
    trackable_ptr<Data> p_i3 = *vec.insert(get_iterator(vec, p_i2), 3);

    std::cout << "dead " << !p_i1 << std::endl;
    std::cout << p_i2->value << std::endl;
    std::cout << p_i3->value << std::endl;

    return 0;
}
```

`get_iterator` / `get_index` allow you to get iterator / index from `trackable_ptr`. And in this way have "non-invalidatable iterators" for `std::vector` compatible containers.


#### `trackable<T>`


`trackable_ptr()` - construct object with default constructor, if possible.

`trackable(Args&&... args)` - in-place construct object.

`trackable(trackable&& other) noexcept` - move object, and update all `trackable_ptr`'s with new address. All `trackable_ptr`'s from `other` now point to this.

`trackable(const trackable&)` - copy object, **does not** update `trackable_ptr`'s.

`trackable& operator=(trackable&&) noexcept` - call destructor, then move constructor.

`trackable& operator=(const trackable&)` - same as copy ctr.

`T* get()` - return object pointer.

`const T* get() const`

`T* operator->()`

`const T* operator->() const`

`T& operator*()`

`const T& operator*() const`

`~trackable() noexcept` - update all `trackable_ptr`'s with new nullptr.

#### `unique_trackable<T>`

Same as `trackable`, but move-only.

Usefull for use in containers. For example, it is not required for `std::vector` implementation to use move instead copy, when both copy and move constructor are available. Though all tested implementations currently prefer move, whenever possible.


#### `trackable_ptr<T>`

`trackable_ptr()` - construct with nullptr

`trackable_ptr(trackable<T>*)` - add this to `trackable` trackers list.

`trackable_ptr(trackable_ptr&& other)` - set other's pointer. Update trackers list with new address. other becomes nullptr.

`trackable_ptr(const trackable_ptr& other)` - copy other pointer, and add this to `trackable` trackers list.

`trackable_ptr& operator=(const trackable_ptr&)` - call destructor, then copy ctr.
`trackable_ptr& operator=(const trackable_ptr&)` - call destructor, then move ctr.

`bool alive() const` - true if not nullptr

`operator bool() const` - return `alive()`

`T* get_trackable()` - return pointer of trackable, that holds object.

`const T* get_trackable() const`

`T* get()` - return object pointer.

`const T* get() const`

`T* operator->()`

`const T* operator->() const`

`T& operator*()`

`const T& operator*() const`

`~trackable_ptr()` - exclude this from trackers list.


### Overhead
 * 1 ptr for `trackable`
 * 3 ptr for `trackable_ptr`
 * O(n) complexity for moving/destroying `trackable`. Where n = `tracker_ptr`s per  `trackable` .
