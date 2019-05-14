#pragma once

namespace tower120::v2{

    namespace detail{
        class trackable_ptr_base;
    }

    class trackable_base{
        friend detail::trackable_ptr_base;
    private:
        detail::trackable_ptr_base* first_ptr = nullptr;

        static detail::trackable_ptr_base* get_next(detail::trackable_ptr_base* ptr) noexcept;
        static trackable_base*& get_obj(detail::trackable_ptr_base* ptr) noexcept;

        template<class Closure>
        static void foreach_ptr(Closure&& closure, detail::trackable_ptr_base* first_ptr) noexcept {
            detail::trackable_ptr_base* ptr = first_ptr;
            while(ptr){
                closure(ptr);
                ptr = get_next(ptr);
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

        void move_ctr(trackable_base& other) noexcept {
            // update trackers with new pointer
            first_ptr       = other.first_ptr;
            other.first_ptr = nullptr;

            foreach_ptr([&](detail::trackable_ptr_base* ptr){
                get_obj(ptr) = this;
            });
        }
    public:

        trackable_base() = default;

        trackable_base(const trackable_base&) noexcept {}
        trackable_base& operator=(const trackable_base&) noexcept { return *this; }

        trackable_base(trackable_base&& other) noexcept{
            move_ctr(other);
        }

        trackable_base& operator=(trackable_base&& other) noexcept {
            if (this == &other) return *this;

            this->~trackable_base();
            move_ctr(other);

            return *this;
        }

        ~trackable_base() noexcept {
            foreach_ptr([](detail::trackable_ptr_base* ptr){
                get_obj(ptr) = nullptr;
            });
        }

    };

}