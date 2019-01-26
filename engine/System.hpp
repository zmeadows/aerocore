#pragma once

#include "UUID.hpp"
#include "Util.hpp"

#include <string>

#include "unstd/ArraySet.hpp"

class System {
protected:
    ArraySet<UUID> m_followed;

    System(const std::string& system_name) : name(system_name) {}

public:
    const std::string name;

    inline bool isFollowing(const UUID& uuid) const {
        return contains(m_followed, uuid);
    }

    inline void follow(const UUID& uuid) {
        insert(m_followed, uuid);
    }

    inline void unfollow(const UUID& uuid) {
        remove(m_followed, uuid);
    }

    virtual void run(float) = 0;

    virtual ~System(void) {}
};
