#pragma once

#include <memory>

#include "unstd/types.hpp"

template <class T>
const T& max(const T& a, const T& b) {
    return (a < b) ? b : a;
}

template <typename T>
using buffer = std::unique_ptr<T[]>;

template <typename T>
buffer<T> allocate_buffer(u64 capacity) {
    return std::unique_ptr<T[]>(new T[capacity]);
}

template <typename ...Types> struct TypeList {};

template <typename T1, typename T2>
T1* memalloc(u64 count) {
    return static_cast<T1*>(malloc(sizeof(T2) * count));
}

template <typename T>
T* memalloc(u64 count) {
    return static_cast<T*>(malloc(sizeof(T) * count));
}
