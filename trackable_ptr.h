#ifndef TRACKABLE_PTR_TRACKABLE_PTR_H
#define TRACKABLE_PTR_TRACKABLE_PTR_H

#include <type_traits>
#include <utility>

template<class>
class trackable;

class trackable_base;

class TrackerBase{
    friend trackable_base;
protected:
    void move_ctr(TrackerBase&& other) noexcept;
    void register_tracker(trackable_base* trackable) noexcept;

    TrackerBase* next = nullptr;
    TrackerBase* prev = nullptr;
    trackable_base* m_trackable = nullptr;

    TrackerBase() noexcept {}
    TrackerBase(trackable_base* trackable) noexcept {
        register_tracker(trackable);
    }
    TrackerBase(TrackerBase&& other) noexcept {
        move_ctr(std::move(other));
    }
    TrackerBase(const TrackerBase& other) noexcept
        :TrackerBase(other.m_trackable)
    {}

    ~TrackerBase() noexcept;
};

template<class T>
class trackable_ptr : TrackerBase{
public:
    // T or derived from T
    template<class R , typename = std::enable_if_t<
            std::is_same_v<R, T>
            || std::is_base_of_v<T, R>
    >>
    trackable_ptr(trackable<R>& trackable) noexcept
        :TrackerBase(&trackable)
    {}

    // T or derived from T, inherited from TrackableBase
    template<class R, typename = std::enable_if_t<
            (std::is_same_v<R, T> || std::is_base_of_v<T, R>)
            && std::is_base_of_v<trackable_base, R>
    >>
    trackable_ptr(R& trackable) noexcept
        :TrackerBase(&trackable)
    {}

    trackable_ptr(trackable_ptr&& other) noexcept
        :TrackerBase(std::move(other))
    {}
    trackable_ptr& operator=(trackable_ptr&& other) noexcept {
        if (this==&other) return *this;

        this->~trackable_ptr();
        move_ctr(std::move(other));

        return *this;
    }

    trackable_ptr(const trackable_ptr& other) noexcept
        :TrackerBase(other)
    {}

    trackable_ptr& operator=(const trackable_ptr& other) noexcept {
        if (this==&other) return *this;

        this->~trackable_ptr();
        register_tracker(other.m_trackable);

        return *this;
    }

    using element_type = std::conditional_t< std::is_base_of_v<trackable_base, T>,
        T,
        trackable<T>
    >;

    element_type* get() const noexcept {
        if constexpr (std::is_base_of_v<trackable_base, T>){
            return static_cast<T*>(m_trackable);
        } else {
            return static_cast<trackable<T>*>(m_trackable);
        }
    }


    explicit operator bool() const noexcept {
        return m_trackable != nullptr;
    }

    element_type* operator->()  const noexcept {
        return get();
    }

    element_type& operator*()  const noexcept {
        return *get();
    }
};


class trackable_base{
    friend TrackerBase;

    void move_ctr(trackable_base&& other){
        // update trackers with new pointer
        tracker_first       = other.tracker_first;
        other.tracker_first = nullptr;

        TrackerBase* tracker = tracker_first;
        while(tracker){
            tracker->m_trackable = this;
            tracker = tracker->next;
        }
    }

    TrackerBase* tracker_first = nullptr;
public:
    trackable_base()noexcept{}
    trackable_base(trackable_base&& other) noexcept
    {
        move_ctr(std::move(other));
    }

    trackable_base& operator=(trackable_base&& other) noexcept {
        if (this == &other) return *this;

        this->~trackable_base();
        move_ctr(std::move(other));

        return *this;
    }

    trackable_base(const trackable_base& other) noexcept {
        // do nothing
    }

    trackable_base& operator=(const trackable_base& other) noexcept {
        if (this == &other) return *this;

        this->~trackable_base();

        return *this;
    }

    ~trackable_base() noexcept{
        TrackerBase* tracker = tracker_first;
        while(tracker){
            tracker->m_trackable = nullptr;
            tracker = tracker->next;
        }
    }
};

template<class T>
class trackable : public trackable_base, public T{
public:
    trackable(){}

    // first call TrackableBase, then T
    trackable(trackable&&) = default;
    trackable(const trackable&) = default;
    trackable& operator=(trackable&&) = default;
    trackable& operator=(const trackable&) = default;


    template<class Arg, class ...Args, typename = std::enable_if_t<
            !std::is_same_v<trackable<T>&&, Arg >
            && !std::is_same_v<const trackable<T>&, Arg >
    >>
    trackable(Arg&& arg, Args&&...args)
        : trackable_base()
        , T(std::forward<Arg>(arg), std::forward<Args>(args)...)
    {}
};




// ---------------------------------------------------------------------------------------
// .cpp part here (just to have single header)

void TrackerBase::register_tracker(trackable_base* trackable) noexcept {
    if (!trackable) return;

    this->m_trackable = trackable;

    TrackerBase* prev_first  = trackable->tracker_first;
    trackable->tracker_first = this;

    if (prev_first) prev_first->prev = this;
    this->next = prev_first;
}

TrackerBase::~TrackerBase() noexcept {
    if(!m_trackable) return;

    if (!prev && !next) {
        // last
        m_trackable->tracker_first = nullptr;
        return;
    }
    else if (!prev){
        // first
        next->prev = nullptr;
        m_trackable->tracker_first = next;
    }
    else if (!next){
        // last
        prev->next = nullptr;
    }
    else {
        // in the middle
        prev->next = next;
        next->prev = prev;
    }

    m_trackable = nullptr;
}

void TrackerBase::move_ctr(TrackerBase &&other) noexcept {
    next = other.next;
    prev = other.prev;

    m_trackable = other.m_trackable;

    other.m_trackable = nullptr;

    if (prev){
        prev->next = this;
    }

    if (next){
        next->prev = this;
    }

    if (m_trackable->tracker_first == &other){
        m_trackable->tracker_first = this;
    }
}


#endif //TRACKABLE_PTR_TRACKABLE_PTR_H
