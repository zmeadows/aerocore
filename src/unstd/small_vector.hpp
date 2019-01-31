#pragma once

#include "unstd/unstdlib.hpp"

template <typename T>
struct SmallVector {
    T* data;
    u64 capacity;
    u64 size = 0;

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

    SmallVector(T* first_elem, u64 capacity) : data(first_elem), capacity(capacity) {}
};

template <typename T, u64 N>
struct SmallVectorN : SmallVector<T> {
    T stack_data[N];

    SmallVectorN() : SmallVector<T>(stack_data, N) {}
};

