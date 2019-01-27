#pragma once

#include "UUID.hpp"
#include "Util.hpp"

#include <string>

#include "unstd/unstdlib.hpp"
#include "unstd/ArraySet.hpp"

#define SUBSCRIBE(...) static constexpr auto subscriptions = TypeList<__VA_ARGS__>()

struct System {
    const std::string name;
    ArraySet<UUID> followed;

    //TODO: make non-member functions
    inline bool is_following(const UUID& uuid) const { return contains(followed, uuid); }
    inline void follow(const UUID& uuid) { insert(followed, uuid); }
    inline void unfollow(const UUID& uuid) { remove(followed, uuid); }

    System(const std::string& system_name) : name(system_name) {}

    System()                         = delete;
    System(const System&)            = delete;
    System& operator=(const System&) = delete;
    System(System&&)                 = delete;
    System& operator=(System&&)      = delete;
};
