#pragma once

#include <cassert>
#include "trackable_base.h"

namespace tower120::v2{
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

        template<class T, bool is_trackable_base = std::is_base_of_v<trackable_base, T>>
        class trackable_ptr_;

        template<class T>
        class trackable_ptr_<T, true> : public detail::trackable_ptr_base {
            static_assert(std::is_base_of_v<trackable_base, T>, "");
            static_assert(!std::is_base_of_v<detail::trackable_tag, T>, "");
        public:
            trackable_ptr_() = default;
            explicit trackable_ptr_(T* obj) noexcept
                : trackable_ptr_base(obj) {}

            trackable_ptr_& operator=(T* obj) noexcept {
                return trackable_ptr_base::operator=(obj);
            }

            T* get() const noexcept {
                return static_cast<T*>(trackable_ptr_base::get());
            }

        protected:
            T* fast_get() const noexcept {
                return get();
            }
        };

        template<class T>
        class trackable_ptr_<T, false> : public detail::trackable_ptr_base {
        public:
            trackable_ptr_() = default;
            explicit trackable_ptr_(trackable<T>* obj) noexcept
                : trackable_ptr_base(obj) {}

            trackable_ptr_& operator=(trackable<T>* obj) noexcept {
                trackable_ptr_base::operator=(obj);
                return *this;
            }

            T* get() const noexcept {
                auto* p = static_cast<trackable<T>*>(trackable_ptr_base::get());
                if (!p) return nullptr;
                return p->get();
            }
        protected:
            T* fast_get() const noexcept {
                auto* p = static_cast<trackable<T>*>(trackable_ptr_base::get());
                assert(p);
                return p->get();
            }
        };

    } // namespace detail


    template<class T>
    class trackable_ptr : public detail::trackable_ptr_<T> {
        using Base = detail::trackable_ptr_<T>;
    public:
        using Base::get;
        using Base::Base;
        using Base::operator=;

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

}