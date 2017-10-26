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

Alos, kinda replacement for `weak_ptr` in terms of object aliveness track. Dead elements have nullptr `trackable_ptr`'s.



### Overhead
 * 1 ptr for Trackable
 * 3 ptr for trackable_ptr (TrackablePtr)

 * O(n) complexity for moving/destroying Trackable. Where n = trackers count.