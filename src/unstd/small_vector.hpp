#pragma once

#include "unstd/unstdlib.hpp"

template <typename T>
struct DynamicArray {
    T* data;
    u64 capacity;
    u64 size;

    T* begin(void) { return data; }
    T* end(void) { return data + size; }

    T& operator[](u64 idx) {
        assert(idx < capacity);
        return *(data + idx);
    }

    void append(const T& item) {
        assert(size < capacity);
        data[size] = item;
        size++;
    }

    SmallVector(T* first_elem, u64 capacity) :
        data(first_elem), capacity(capacity), size(0) {}
};

template <typename T, u64 N>
struct DynamicStackaRray : SmallVector<T> {
    // use union with T* for heap-allocated overflow
    T stack_data[N];

    SmallVectorN() : SmallVector<T>(stack_data, N) {}
};

