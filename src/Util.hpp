#pragma once

#include <string>
#include <cmath>
#include <vector>
#include <iostream>
#include <istream>
#include <fstream>
#include <assert.h>

#include "unstd/types.hpp"

const f32 TWOPI = 2.f * static_cast<f32>(3.1415926535897932384);
const f32 PI = static_cast<f32>(3.1415926535897932384);

#ifdef DODEBUG
#define DEBUG(x) \
    (std::cout << std::boolalpha << "[" << __FILE__ << "::" << __LINE__ << "] :: " << x << std::endl)
#else
#define DEBUG(x)
#endif

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

inline bool rough_equals(f32 x, f32 y, f32 delta) {
    return fabs(x - y) < delta;
}

template <class T>
constexpr
std::string
type_name()
{
    using namespace std;
#ifdef __clang__
    std::string p = __PRETTY_FUNCTION__;
    return std::string(p.data() + 34, p.size() - 34 - 1);
    //return std::string(p.data() + 57, p.size() - 57 - 1);
#elif defined(__GNUC__)
    std::string p = __PRETTY_FUNCTION__;
#  if __cplusplus < 201402
    return std::string(p.data() + 36, p.size() - 36 - 1);
#  else
    return std::string(p.data() + 49, p.find(';', 49) - 49);
#  endif
#elif defined(_MSC_VER)
    std::string p = __FUNCSIG__;
    return std::string(p.data() + 113, p.size() - 113 - 7);
#endif
}

inline f32 signum(f32 x) {
    return static_cast<f32>(x > 0) - static_cast<f32>(x < 0);
}

inline f32 rotate(f32& old_angle, f32 delta) {
    const f32 new_angle = fmod(old_angle + delta, TWOPI);
    return (new_angle < 0) ? new_angle + TWOPI : new_angle;
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

template <typename T>
inline bool vector_contains(const std::vector<T>& cont, const T& data) {
    return std::find(cont.begin(), cont.end(), data) != cont.end();
}

template <typename Container, typename T>
inline bool contains(const Container& cont, const T& data) {
    return cont.find(data) != cont.end();
}
