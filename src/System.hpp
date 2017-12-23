#pragma once

#include "UUID.hpp"

class ComponentManager;

class System {
protected:
    UUIDSet m_followed;

    ComponentManager* const CM;

    System(const std::string& name_, ComponentManager* const CM_) : CM(CM_), name(name_) {}

    // virtual void preRunHook(void) {}
    // virtual void runOnEntity(const UUID& uuid, float dt) = 0;
    // virtual void postRunHook(void) {}

public:
    virtual void run(float dt) = 0;
    // preRunHook();
    // for (const UUID& uuid : m_followed) {
    //     runOnEntity(uuid, dt);
    //     }
    //     postRunHook();
    // }

    const std::string name;

    bool isFollowing(const UUID& uuid) const { return static_cast<bool>(m_followed.contains(uuid)); }

    void follow(const UUID& uuid) {
        m_followed.insert(uuid);
        DEBUG(name << " system followed UUID: " << uuid);
    }

    void unfollow(const UUID& uuid) {
        m_followed.erase(uuid);
        DEBUG(name << " system unfollowed UUID: " << uuid);
    }
};
