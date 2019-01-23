#pragma once

template <typename T>
void swap(T& a, T& b) {
    T tmp = std::move(a);
    a = std::move(b);
    b = std::move(tmp);
}

template <class T>
const T& max(const T& a, const T& b) {
    return (a < b) ? b : a;
}

struct NonCopyable {
    NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
    NonCopyable& operator=(NonCopyable) = delete;
    NonCopyable& operator=(NonCopyable&&) = delete;
    NonCopyable(NonCopyable&&) = delete;
};

