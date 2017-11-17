#pragma once

#include <set>
#include <memory>

#include "UUID.hpp"

class ComponentManager;

class System {
private:
  std::set<UUID> m_followed;

protected:
  ComponentManager* const CM;
  virtual void runInternal(const UUID& uuid) = 0;

  System(const std::string &name_, ComponentManager *const CM_)
      : name(name_), CM(CM_) {}

public:
  void run(void) {
    for (const UUID& uuid : m_followed)
      runInternal(uuid);
  }

  const std::string name;

  bool isFollowing(const UUID &uuid) const {
    return static_cast<bool>(m_followed.count(uuid));
  }

  void follow(const UUID& uuid) {
     m_followed.insert(uuid);
     DEBUG (name << " system followed UUID: " << uuid);
  }

  void unfollow(const UUID& uuid) { m_followed.erase(uuid); }
};
