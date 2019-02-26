#pragma once

template <typename T>
class Slice {
    const T* const data;
    size_t count;
public:
    Slice(const T* const _data, size_t _count) : data(_data), count(_count) {}

    const T* const begin(void) const { return reinterpret_cast<const T* const>(this->data); }
    const T* const end(void) const { return reinterpret_cast<const T* const>(this->data) + this->count; }
};
