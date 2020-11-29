#pragma once

#include <cstdint>

class Address
{
public:
    Address() noexcept = default;
    Address(const uintptr_t _ptr) noexcept : ptr(_ptr) {};
    Address(const void* _ptr) noexcept : ptr(reinterpret_cast<uintptr_t> (_ptr)) {};

    operator uintptr_t(void) const noexcept;
    explicit operator void* (void) const noexcept;

    bool isValid(void) const noexcept;

    template <typename T = uintptr_t>
    T get(void) const noexcept
    {
        return static_cast<T>(ptr);
    }

    Address addOffset(const uint32_t offset) noexcept;

    //Address operator+(const Address& b) {
    //    Address address = Address(this->ptr+b.get());
    //    return address;
    //}

    //Address operator+(const uintptr_t b) {
    //    Address address = Address(this->ptr + b);
    //    return address;
    //}

protected:
    uintptr_t ptr;
};
