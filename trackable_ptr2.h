#ifndef TRACKABLE_PTR_TRACKABLE_PTR_H
#define TRACKABLE_PTR_TRACKABLE_PTR_H

#include <type_traits>
#include <utility>

template<class>
class Trackable;

class TrackableBase;

class TrackerBase{
    friend TrackableBase;
protected:
    TrackerBase* next = nullptr;
    TrackerBase* prev = nullptr;     // possible to remove, but move will cost O(n)
    TrackableBase* trackable = nullptr;

    TrackerBase(TrackableBase* trackable);
    TrackerBase(TrackerBase&& other);
    TrackerBase(const TrackerBase& other)
        :TrackerBase(other.trackable)
    {}
    ~TrackerBase();
};

template<class T>
class Tracker : TrackerBase{
public:
    Tracker(Trackable<T>& trackable)
        :TrackerBase(&trackable)
    {}

    Trackable<T>* get(){
        return static_cast<Trackable<T>*>(trackable);
    }

    operator bool() const {
        return trackable == nullptr;
    }

    Trackable<T>* operator->() {
        return get();
    }
};


class TrackableBase{
    friend TrackerBase;
protected:
    TrackerBase* tracker_head = nullptr;
public:
    TrackableBase(){}
    TrackableBase(TrackableBase&& other)
    {
        tracker_head = other.tracker_head;
        other.tracker_head = nullptr;

        TrackerBase* tracker = tracker_head;
        while(tracker){
            tracker->trackable = this;
            tracker = tracker->next;
        }
    }

    // TODO: move assign


    TrackableBase(const TrackableBase& other){
        // do nothing
    }

    ~TrackableBase(){
        TrackerBase* tracker = tracker_head;
        while(tracker){
            tracker->trackable = nullptr;
            tracker = tracker->next;
        }
    }
};

template<class T>
class Trackable : public TrackableBase, public T{
public:
    Trackable(){}
    Trackable(Trackable&&) = default;

    template<class Arg, class ...Args, typename = std::enable_if_t<
            !std::is_same_v<Trackable<T>&&, Arg >
            && !std::is_same_v<const Trackable<T>&, Arg >
    >>
    Trackable(Arg&& arg, Args&&...args)
        : TrackableBase()
        , T(std::forward<Arg>(arg), std::forward<Args>(args)...)
    {}
};


#endif //TRACKABLE_PTR_TRACKABLE_PTR_H
