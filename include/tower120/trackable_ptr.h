#pragma once

#include <cassert>
#include "trackable_base.h"
#include "trackable.h"

namespace tower120{
    namespace detail{

        class trackable_ptr_base {
            friend trackable_base;
        private:
            trackable_base* obj;
            trackable_ptr_base* next;
            trackable_ptr_base* prev;

            void init(trackable_base* obj) noexcept {
                this->obj = obj;
                if (!obj) return;

                auto& first_ptr = this->obj->first_ptr;

                this->prev = nullptr;
                this->next = first_ptr;

                if (first_ptr) first_ptr->prev = this;
                first_ptr = this;
            }

            void move_ctr(trackable_ptr_base&& other) noexcept {
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

        protected:
            trackable_base* get() const noexcept {
                return obj;
            }

        public:
            trackable_ptr_base() noexcept : obj(nullptr) {}

        protected:
            explicit trackable_ptr_base(trackable_base* obj) noexcept
            {
                init(obj);
            }
            trackable_ptr_base& operator=(trackable_base* obj) noexcept
            {
                this->~trackable_ptr_base();
                init(obj);
                return *this;
            }

        public:
            trackable_ptr_base(const trackable_ptr_base& other) noexcept
            {
                init(other.obj);
            }
            trackable_ptr_base(trackable_ptr_base&& other) noexcept {
                move_ctr(std::move(other));
            }

            trackable_ptr_base& operator=(const trackable_ptr_base& other) noexcept {
                this->~trackable_ptr_base();
                init(other.obj);
                return *this;
            }
            trackable_ptr_base& operator=(trackable_ptr_base&& other) noexcept {
                this->~trackable_ptr_base();
                move_ctr(std::move(other));
                return *this;
            }

            ~trackable_ptr_base() noexcept {
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

    } // namespace detail


    template<class T>
    class trackable_ptr : public detail::trackable_ptr_base {
        static_assert(!std::is_reference<T>::value && !std::is_pointer<T>::value, "T must be type.");

        template<typename> friend class trackable_ptr;

        template<class T_ = T>
        static auto is_trackable_base(){ return std::integral_constant<bool,
            std::is_base_of<trackable_base, T_>::value && !std::is_base_of<detail::trackable_tag, T_>::value>{};
        }

        template<class>
        T* get(/*is_trackable_base*/ std::true_type) const noexcept {
            return static_cast<T*>(trackable_ptr_base::get());
        }
        template<class>
        T* get(/*is_trackable_base*/ std::false_type) const noexcept {
            auto* p = static_cast<trackable<std::remove_const_t<T>>*>(trackable_ptr_base::get());
            if (!p) return nullptr;
            return p->get();
        }

        template<class>
        T* fast_get(/*is_trackable_base*/ std::true_type) const noexcept {
            return get();
        }
        template<class>
        T* fast_get(/*is_trackable_base*/ std::false_type) const noexcept {
            decltype(auto) p = static_cast<trackable<std::remove_const_t<T>>*>(trackable_ptr_base::get());
            assert(p);
            return p->get();
        }
        T* fast_get() const noexcept {
            return fast_get<void>(is_trackable_base());
        }

        template<class>
        T* get_trackable(/*is_trackable_base*/ std::true_type) const noexcept {
            return get();
        }
        template<class>
        trackable<T>* get_trackable(/*is_trackable_base*/ std::false_type) const noexcept {
            return static_cast<trackable<T>*>(trackable_ptr_base::get());
        }

        static void must_be_const(){
            static_assert(std::is_const<T>::value, "T must be const!");
        }

        template<class Y>
        static void must_be_convertible(){
            static_assert(std::is_constructible<T&, Y&>::value, "trackable_ptr implicit conversion error.");
        }

    public:
        trackable_ptr() noexcept = default;
        trackable_ptr(const trackable_ptr&) noexcept = default;
        trackable_ptr(trackable_ptr&&) noexcept = default;
        trackable_ptr& operator=(const trackable_ptr&) noexcept = default;
        trackable_ptr& operator=(trackable_ptr&&) noexcept = default;

        // conversion copy ctr
        template<class Y>
        trackable_ptr(const trackable_ptr<Y>& obj) noexcept
            : trackable_ptr_base(obj)
        {
            must_be_convertible<Y>();
        }
        // conversion move ctr
        template<class Y>
        trackable_ptr(trackable_ptr<Y>&& obj) noexcept
            : trackable_ptr_base(std::move(obj))
        {
            must_be_convertible<Y>();
        }
        // conversion copy assign
        template<class Y>
        trackable_ptr& operator=(const trackable_ptr<Y>& obj) noexcept
        {
            must_be_convertible<Y>();
            trackable_ptr_base::operator=(obj);
            return *this;
        }
        // conversion move assign
        template<class Y>
        trackable_ptr& operator=(trackable_ptr<Y>&& obj) noexcept
        {
            must_be_convertible<Y>();
            trackable_ptr_base::operator=(std::move(obj));
            return *this;
        }

        explicit trackable_ptr(std::remove_const_t<T>* obj) noexcept
            : trackable_ptr_base(obj) {}
        trackable_ptr& operator=(std::remove_const_t<T>* obj) noexcept {
            trackable_ptr_base::operator=(obj);
            return *this;
        }
        explicit trackable_ptr(trackable<std::remove_const_t<T>>* obj) noexcept
            : trackable_ptr_base(obj) {}
        trackable_ptr& operator=(trackable<std::remove_const_t<T>>* obj) noexcept {
            trackable_ptr_base::operator=(obj);
            return *this;
        }
        // const ctrs
        explicit trackable_ptr(const std::remove_const_t<T>* obj) noexcept
            : trackable_ptr_base(const_cast<std::remove_const_t<T>*>(obj)) {
            must_be_const();
        }
        trackable_ptr& operator=(const std::remove_const_t<T>* obj) noexcept {
            must_be_const();
            trackable_ptr_base::operator=(const_cast<std::remove_const_t<T>*>(obj));
            return *this;
        }
        explicit trackable_ptr(const trackable<std::remove_const_t<T>>* obj) noexcept
            : trackable_ptr_base(const_cast<trackable<std::remove_const_t<T>>*>(obj)) {
            must_be_const();
        }
        trackable_ptr& operator=(const trackable<std::remove_const_t<T>>* obj) noexcept {
            must_be_const();
            trackable_ptr_base::operator=(const_cast<trackable<std::remove_const_t<T>>*>(obj));
            return *this;
        }

        /// return T* if T is trackable_base
        ///   else trackable<T>*
        auto* get_trackable() const noexcept {
            return get_trackable<void>(is_trackable_base());
        }

        T* get() const noexcept {
            return get<void>(is_trackable_base());
        }

        T* operator->() const noexcept {
            assert(*this);
            return this->fast_get();
        }

        T& operator*() const noexcept {
            assert(*this);
            return *this->fast_get();
        }

        explicit operator bool() const noexcept {
            return detail::trackable_ptr_base::get() != nullptr;
        }

        template<class Y>
        bool operator==(const trackable_ptr<Y>& other) const noexcept {
            must_be_convertible<Y>();
            return detail::trackable_ptr_base::get() == other.detail::trackable_ptr_base::get();
        }
        template<class Y>
        bool operator!=(const trackable_ptr<Y>& other) const noexcept {
            must_be_convertible<Y>();
            return detail::trackable_ptr_base::get() != other.detail::trackable_ptr_base::get();
        }
    };


#if __cpp_deduction_guides
    // C++17 Deduction guides
    template<class T>
    trackable_ptr(T*) -> trackable_ptr<T>;
    template<class T>
    trackable_ptr(const T*) -> trackable_ptr<const T>;
    template<class T>
    trackable_ptr(trackable<T>*) -> trackable_ptr<T>;
    template<class T>
    trackable_ptr(const trackable<T>*) -> trackable_ptr<const T>;
#endif

    detail::trackable_ptr_base* trackable_base::get_next(detail::trackable_ptr_base* ptr) noexcept {
        return ptr->next;
    }
    trackable_base*& trackable_base::get_obj(detail::trackable_ptr_base* ptr) noexcept {
        return ptr->obj;
    }

}