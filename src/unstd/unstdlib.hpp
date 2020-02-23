#pragma once

#include <memory>
#include <atomic>

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

template <typename T1, typename T2>
T1* memallocz(u64 count) {
    return static_cast<T1*>(calloc(count, sizeof(T2)));
}

template <typename T>
T* memallocz(u64 count) {
    return static_cast<T*>(calloc(count, sizeof(T)));
}

class TypeIndexer {
    std::atomic<u32> m_nextIndex;

    inline u32 newIndex(void) {
        return m_nextIndex.fetch_add(1);
    }

public:

    template <typename T>
    inline u32 operator()(void) {
        static const u32 idx = newIndex();
        return idx;
    }
};

// u64 modulo(s64 i, s64 n) {
//     return static_cast<u64>((i % n + n) % n);
//}
