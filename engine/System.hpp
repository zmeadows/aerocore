#pragma once

#include "UUID.hpp"
#include "Util.hpp"

#include <string>

class System {
protected:
    UUIDSet m_followed;

    System(const std::string& system_name) : name(system_name) {}

public:
    const std::string name;

    inline bool isFollowing(const UUID& uuid) const {
        return static_cast<bool>(m_followed.contains(uuid));
    }

    inline void follow(const UUID& uuid) {
        m_followed.insert(uuid);
    }

    inline void unfollow(const UUID& uuid) {
        m_followed.erase(uuid);
    }

    inline UUIDSet::const_iterator unfollow(UUIDSet::const_iterator uuid_iter) {
        return m_followed.erase(uuid_iter);
    }

    virtual void run(float) = 0;

    virtual ~System(void) {}
};
