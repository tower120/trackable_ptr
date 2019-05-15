#pragma once

#include <cassert>
#include "trackable_base.h"
#include "trackable.h"

namespace tower120{ namespace v2{
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
        static_assert(!std::is_reference_v<T> && !std::is_pointer_v<T>, "T must be type.");

        static const constexpr bool is_trackable_base =
            std::is_base_of<trackable_base, T>::value && !std::is_base_of<detail::trackable_tag, T>::value;

        T* fast_get() const noexcept {
            if constexpr (is_trackable_base){
                return get();
            } else {
                decltype(auto) p = static_cast<trackable<T>*>(trackable_ptr_base::get());
                assert(p);
                return p->get();
            }
        }

        void must_be_const(){ static_assert(std::is_const_v<T>, "T must be const!"); }

    public:
        trackable_ptr() = default;
        trackable_ptr(std::remove_const_t<T>* obj) noexcept
            : trackable_ptr_base(obj) {}
        trackable_ptr& operator=(std::remove_const_t<T>* obj) noexcept {
            trackable_ptr_base::operator=(obj);
            return *this;
        }
        trackable_ptr(trackable<std::remove_const_t<T>>* obj) noexcept
            : trackable_ptr_base(obj) {}
        trackable_ptr& operator=(trackable<std::remove_const_t<T>>* obj) noexcept {
            trackable_ptr_base::operator=(obj);
            return *this;
        }
        // const ctrs
        trackable_ptr(const std::remove_const_t<T>* obj) noexcept
            : trackable_ptr_base(const_cast<std::remove_const_t<T>*>(obj)) {
            must_be_const();
        }
        trackable_ptr& operator=(const std::remove_const_t<T>* obj) noexcept {
            must_be_const();
            trackable_ptr_base::operator=(const_cast<std::remove_const_t<T>*>(obj));
            return *this;
        }
        trackable_ptr(trackable<const std::remove_const_t<T>>* obj) noexcept
            : trackable_ptr_base(obj) {
            must_be_const();
        }
        trackable_ptr& operator=(trackable<const std::remove_const_t<T>>* obj) noexcept {
            must_be_const();
            trackable_ptr_base::operator=(obj);
            return *this;
        }

        T* get() const noexcept {
            if constexpr (is_trackable_base){
                return static_cast<T*>(trackable_ptr_base::get());
            } else {
                auto* p = static_cast<trackable<T>*>(trackable_ptr_base::get());
                if (!p) return nullptr;
                return p->get();
            }
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

        // TODO: add cross-const comparison

        bool operator==(const trackable_ptr& other) const noexcept {
            return detail::trackable_ptr_base::get() == other.detail::trackable_ptr_base::get();
        }
        bool operator!=(const trackable_ptr& other) const noexcept {
            return detail::trackable_ptr_base::get() != other.detail::trackable_ptr_base::get();
        }
    };



    detail::trackable_ptr_base* trackable_base::get_next(detail::trackable_ptr_base* ptr) noexcept {
        return ptr->next;
    }
    trackable_base*& trackable_base::get_obj(detail::trackable_ptr_base* ptr) noexcept {
        return ptr->obj;
    }

}}