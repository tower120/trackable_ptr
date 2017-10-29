# trackable_ptr

Trackable pointer. When trackable object moved/destroyed, trackers updated with new object's pointer.

[Live example](http://coliru.stacked-crooked.com/a/c6a2e71ea86f8902)

Allow to have stable pointer on any movable object (in single-threaded environment).
Like:

```c++
struct Data{
    int x,y,z;
};

std::vector< trackable<Data> > list1;

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


Does not increase object lifetime (like `shared_ptr`)

Also, kinda replacement for `weak_ptr` in terms of object aliveness track. Dead elements have nullptr `trackable_ptr`'s.



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

`get_iterator` / `get_index` allow you to get iterator / index from `trackable_ptr`. And in this way have "non-invalidatable iterators" for `std::vector`.



#### Inheriting `trackable_base`

Instead of wrapping your class with `trackable`, like `trackable<MyClass>`; you may inerit `trackable_base`, instead:

```c++
struct MyClass : trackable_base{}

std::vector<MyClass> vec;
trackable_ptr<MyClass> first = {vec.emplace_back()};
```



### Overhead
 * 1 ptr for `trackable`
 * 3 ptr for `trackable_ptr`

 * O(n) complexity for moving/destroying `trackable`. Where n = `tracker_ptr`s per  `trackable` .
