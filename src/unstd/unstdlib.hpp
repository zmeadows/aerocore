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


