#include "System.hpp"

#include "Util.hpp"

bool System::isFollowing(const UUID& uuid) const {
    return static_cast<bool>(m_followed.contains(uuid));
}

void System::follow(const UUID& uuid) {
    m_followed.insert(uuid);
    DEBUG(name << " system followed UUID: " << uuid);
}

void System::unfollow(const UUID& uuid) {
    m_followed.erase(uuid);
    DEBUG(name << " system unfollowed UUID: " << uuid);
}

void System::run(float) {}
