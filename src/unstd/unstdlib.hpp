#pragma once

#include <memory>

#include "unstd/types.hpp"
#include "unstd/Defer.hpp"

template <class T>
const T& max(const T& a, const T& b) {
    return (a < b) ? b : a;
}

template <class T>
const T& min(const T& a, const T& b) {
    return (a > b) ? b : a;
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

// u64 modulo(s64 i, s64 n) {
//     return static_cast<u64>((i % n + n) % n);
//}
