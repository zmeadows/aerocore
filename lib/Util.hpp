#pragma once

#include <string>
#include <algorithm>
#include <cmath>

#include "Typedef.hpp"

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

inline bool rough_equals(float x, float y, float delta) {
    return fabs(x - y) < delta;
}

template <class T>
constexpr
std::string_view
type_name()
{
    using namespace std;
#ifdef __clang__
    string_view p = __PRETTY_FUNCTION__;
    return string_view(p.data() + 34, p.size() - 34 - 1);
    //return string_view(p.data() + 57, p.size() - 57 - 1);
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

template <typename Container, typename T>
inline bool contains(const Container& cont, const T& data) {
    return cont.find(data) != cont.end();
}

template <typename T>
inline bool vector_contains(const std::vector<T>& cont, const T& data) {
    return std::find(cont.begin(), cont.end(), data) != cont.end();
}

