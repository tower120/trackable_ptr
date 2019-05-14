#pragma once

#include <cassert>

namespace tower120{

    template<class>
    class trackable_ptr;

    template<class T>
    class trackable{
        friend trackable_ptr<T>;

        T value;
        trackable_ptr<T>* first_ptr = nullptr;

        template<class Closure>
        static void foreach_ptr(Closure&& closure, trackable_ptr<T>* first_ptr){
            trackable_ptr<T>* ptr = first_ptr;
            while(ptr){
                closure(*ptr);
                ptr = ptr->next;
            }
        }
        template<class Closure>
        void foreach_ptr(Closure&& closure){
            foreach_ptr(std::forward<Closure>(closure), first_ptr);
        }
        template<class Closure>
        void foreach_ptr(Closure&& closure) const {
            foreach_ptr(std::forward<Closure>(closure), first_ptr);
        }

        void move_ctr(trackable& other) noexcept {
            // update trackers with new pointer
            first_ptr       = other.first_ptr;
            other.first_ptr = nullptr;

            foreach_ptr([&](trackable_ptr<T>& ptr){
                ptr.obj = this;
            });
        }

    public:
        trackable() = default;

        explicit trackable(T&& value) noexcept
            : value(std::move(value))
        {}

        template<class Arg, class ...Args,
            typename = std::enable_if_t<
                sizeof...(Args) == 0 &&
                !std::is_same<trackable<T>, std::decay_t<Arg> >::value>>
        explicit trackable(Arg&& arg, Args&&...args) noexcept
            : value(std::forward<Arg>(arg), std::forward<Args>(args)...)
        {}

        trackable(const trackable& other) noexcept
            : value(other.value)
        {}

        trackable& operator=(const trackable& other) noexcept {
            value = other.value;
            return *this;
        }

        trackable(trackable&& other) noexcept
            : value(std::move(other.value))
        {
            move_ctr(other);
        }

        trackable& operator=(trackable&& other) noexcept {
            if (this == &other) return *this;

            this->~trackable();
            value = std::move(other.value);
            move_ctr(other);

            return *this;
        }

        // TODO: conditional noexcept

        std::size_t ptrs_count() const noexcept {
            std::size_t count = 0;
            foreach_ptr([&](const trackable_ptr<T>&){
                count++;
            });
            return count;
        }

        using type = T;

        T* get() noexcept {
            return &value;
        }
        const T* get() const noexcept{
            return &value;
        }

        T& operator*(){
            return value;
        }
        const T& operator*() const{
            return value;
        }

        T* operator->() noexcept {
            return &value;
        }
        const T* operator->() const noexcept{
            return &value;
        }

        ~trackable() noexcept{
            foreach_ptr([](trackable_ptr<T>& ptr){
                ptr.obj = nullptr;
            });
        }
    };


    template<class T>
    class unique_trackable : public trackable<T> {
    public:
        using trackable<T>::trackable;

        unique_trackable(unique_trackable&&) noexcept = default;
        unique_trackable& operator=(unique_trackable&&) noexcept = default;

        unique_trackable(const unique_trackable&) = delete;
        unique_trackable& operator=(const unique_trackable&) = delete;
    };



    template<class T>
    class trackable_ptr{
        friend trackable<T>;

        trackable<T>* obj;

        trackable_ptr* prev;
        trackable_ptr* next;

        void init(trackable<T>* obj) noexcept {
            this->obj = obj;
            if (!obj) return;

            auto& first_ptr = this->obj->first_ptr;

            this->prev = nullptr;
            this->next = first_ptr;

            if (first_ptr) first_ptr->prev = this;
            first_ptr = this;
        }

        void move_ctr(trackable_ptr&& other) noexcept{
            obj = other.obj;
            if(!obj) return;
            other.obj = nullptr;

            prev = other.prev;
            next = other.next;

            if (prev) {
                prev->next = this;
            } else{
                assert(obj->first_ptr == &other);
                obj->first_ptr = this;
            }

            if (next) next->prev = this;
        }
    public:
        trackable_ptr() noexcept : obj(nullptr) {}
        explicit trackable_ptr(trackable<T>* obj) noexcept
        {
            init(obj);
        }
        trackable_ptr& operator=(trackable<T>* obj) noexcept
        {
            this->~trackable_ptr();
            init(obj);
            return *this;
        }

        trackable_ptr(const trackable_ptr& other) noexcept
        {
            init(other.obj);
        }
        trackable_ptr(trackable_ptr&& other) noexcept {
            move_ctr(std::move(other));
        }

        trackable_ptr& operator=(trackable_ptr&& other) noexcept {
            this->~trackable_ptr();
            move_ctr(std::move(other));
            return *this;
        }
        trackable_ptr& operator=(const trackable_ptr& other) noexcept {
            this->~trackable_ptr();
            init(other.obj);
            return *this;
        }

        trackable<T>* get_trackable() noexcept {
            return obj;
        }
        const trackable<T>* get_trackable() const noexcept {
            return obj;
        }

        T* get() noexcept {
            return obj->get();
        }
        const T* get() const noexcept{
            return obj->get();
        }

        T* operator->() noexcept {
            return get();
        }
        const T* operator->() const noexcept {
            return get();
        }

        T& operator*() noexcept {
            return *get();
        }
        const T& operator*() const noexcept {
            return *get();
        }

        explicit operator bool() const noexcept {
            return obj != nullptr;
        }

        bool operator==(const trackable_ptr& other) const noexcept {
            return get() == other.get();
        }
        bool operator!=(const trackable_ptr& other) const noexcept {
            return get() != other.get();
        }

        ~trackable_ptr() noexcept {
            if(!obj) return;

            if (prev) {
                prev->next = next;
            } else {
                assert(obj->first_ptr == this);
                obj->first_ptr = next;
            }

            if (next) next->prev = prev;
        }
    };

}