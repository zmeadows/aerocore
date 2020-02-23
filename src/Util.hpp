#pragma once

#include <string>
#include <cmath>
#include <vector>
#include <iostream>
#include <istream>
#include <fstream>
#include <assert.h>

#include "unstd/types.hpp"

static const f32 TWOPI = 2.f * static_cast<f32>(3.1415926535897932384);
static const f32 PI = static_cast<f32>(3.1415926535897932384);

// #define DEBUG(x) \
//     (std::cout << std::boolalpha << "[" << __FILE__ << "::" << __LINE__ << "] :: " << x << std::endl)
#define DEBUG(x)

inline u32 modulo(s32 i, s32 n) {
    return static_cast<u32>((i % n + n) % n);
}

inline u32 unsign(s32 x) {
    assert(x >= 0);
    return static_cast<u32>(x);
}

inline std::string asset_path(std::string local_path) {
    return std::string(ASSET_PATH) + local_path;
};


template <class T>
constexpr
std::string_view
type_name()
{
    using namespace std;
#ifdef __clang__
    string_view p = __PRETTY_FUNCTION__;
    return string_view(p.data() + 34, p.size() - 34 - 1);
#elif defined(__GNUC__)
    string_view p = __PRETTY_FUNCTION__;
#  if __cplusplus < 201402
    return string_view(p.data() + 36, p.size() - 36 - 1);
#  else
    return string_view(p.data() + 49, p.find(';', 49) - 49);
#  endif
#elif defined(_MSC_VER)
    string_view p = __FUNCSIG__;
    return string_view(p.data() + 84, p.size() - 84 - 7);
#endif
}

inline f32 signum(f32 x) {
    return static_cast<f32>(x > 0) - static_cast<f32>(x < 0);
}

template <typename T>
T clamp(T value, T low, T high) {
    if (value < low) {
        return low;
    } else if (value > high) {
        return high;
    } else {
        return value;
    }
}
ault
template <typename T>
inline bool vector_contains(const std::vector<T>& cont, const T& data) {
    return std::find(cont.begin(), cont.end(), data) != cont.end();
}
