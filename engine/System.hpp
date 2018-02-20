#pragma once

#include "UUID.hpp"

class ComponentManager;

class System {
protected:
    UUIDSet m_followed;

    System(const std::string& system_name) : name(system_name) {}

public:
    const std::string name;

    void follow(const UUID& uuid);
    void unfollow(const UUID& uuid);
    bool isFollowing(const UUID& uuid) const;

    virtual void run(float);

    virtual ~System(void) {}
};
