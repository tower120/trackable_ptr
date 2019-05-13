#pragma once

struct test_struct{
    int i;
    static constexpr const int sentinel = -1;

    test_struct() = default;
    test_struct(int i) noexcept : i(i) {}
    test_struct(const test_struct& other) noexcept
    {
        i = other.i;
    }
    test_struct(test_struct&& other) noexcept
    {
        i = other.i;
        other.i = sentinel;
    }

    test_struct& operator=(const test_struct& other) noexcept
    {
        i = other.i;
        return *this;
    }
    test_struct& operator=(test_struct&& other) noexcept
    {
        i = other.i;
        other.i = sentinel;
        return *this;
    }

    ~test_struct() noexcept
    {
        i = sentinel;
    }
};
