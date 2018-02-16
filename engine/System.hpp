#pragma once

#include "UUID.hpp"

class ComponentManager;

class System {
protected:
    UUIDSet m_followed;

    ComponentManager* const CM;

    System(const std::string& name_, ComponentManager* const CM_)
        : CM(CM_), name(name_) {}

public:
    const std::string name;

    void follow(const UUID& uuid);
    void unfollow(const UUID& uuid);
    bool isFollowing(const UUID& uuid) const;

    virtual void run(float);

    virtual ~System(void) {}
};
