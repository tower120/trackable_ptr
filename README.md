# trackable_ptr

Trackable pointer. When `trackable` object moved/destroyed, `trackable_ptr`s updated with new object's pointer.

Allow to have stable pointer on any movable object (in single-threaded environment). Objects may be stack allocated.

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

```cpp
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

std::vector<Box> boxes;

// Box can be moved around. Corner::box always valid.
// trackable_ptr<Box> can be stored in lambda callback.

Box& active_box = boxes[i];
on_some_event([box = trackable_ptr<Box>(&active_box)](){
    if (!box) return;
    std::cout << box->lt.x;
});

```

#### Behavior

On `trackable` destruction - all `trackable_ptr`s becomes nullptr.

On `trackable` move - all `trackable_ptr`s updates with new object location.

On `trackable` copy - `trackable_ptr`s unaffected.


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


#### `trackable_base`

Inherit this, if you want your class to be compatible with `trackable_ptr`.

```cpp
struct MyClass : trackable_base {}

MyClass m;
trackable_ptr<MyClass> p = &m;

```

#### `trackable<T>`

```cpp
trackable<int> i;
trackable_ptr<int> p = &i;

auto i2 = std::move(i);
assert(p.get() == i2.get());

```

 * `trackable()` - construct object with default constructor, if possible.
 * `trackable(T&&)` - conversion constructor.
 * `trackable(Args&&... args)` - in-place construct object.
 * `trackable(trackable&& other)`
 * `trackable(const trackable&)`
 * `trackable& operator=(trackable&&)`
 * `trackable& operator=(const trackable&)`
 * `T* get()` - return object pointer.
 * `const T* get() const`
 * `T* operator->()`
 * `const T* operator->() const`
 * `T& operator*()`
 * `const T& operator*() const`
 * `~trackable()` - update all `trackable_ptr`'s with new nullptr.

#### `unique_trackable<T>`

Same as `trackable`, but move-only.

Usefull for use in containers. For example, it is not required for `std::vector` implementation to use move instead copy, when both copy and move constructor are available. Though all tested implementations currently prefer move, whenever possible.


#### `trackable_ptr<T>`

 * `trackable_ptr()` - construct with nullptr
 * `trackable_ptr(T*)`
 * `trackable_ptr(trackable<T>*)`
 * `operator bool() const` - return true if not nullptr
 * `T* get() const` - return object pointer.
 * `T* operator->() const`
 * `T& operator*() const`
 * `~trackable_ptr()` - exclude this from trackers list.


### Overhead
 * 1 ptr for `trackable`
 * 3 ptr for `trackable_ptr`
 * O(n) complexity for moving/destroying `trackable`. Where n = `tracker_ptr`s per `trackable`.
