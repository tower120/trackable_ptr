#pragma once

#include "trackable_base.h"

namespace tower120{ namespace v2{

    namespace detail{
        struct trackable_tag{};
    }

    template<class T>
    class trackable : public trackable_base, detail::trackable_tag{
        /*static_assert(!std::is_base_of_v<T, trackable_base>,
            "Your type already inherit trackable_base. You don't need wrap it with trackable.");*/
        T value;
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

        using type = T;

        T* get() noexcept {
            return &value;
        }
        const T* get() const noexcept{
            return &value;
        }

        T& operator*() noexcept{
            return value;
        }
        const T& operator*() const noexcept{
            return value;
        }

        T* operator->() noexcept {
            return &value;
        }
        const T* operator->() const noexcept{
            return &value;
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

}}