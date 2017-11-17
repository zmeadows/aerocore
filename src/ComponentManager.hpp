#pragma once

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <set>

#include "Base.hpp"
#include "Sprite.hpp"
#include "System.hpp"
#include "UUID.hpp"
#include "Util.hpp"

class ComponentManager {
private:
  using UUIDCompMap = std::unordered_map<UUID::rep, void *>;
  using ComponentIndex = uint_fast16_t;

  ComponentIndex nextIndex = 0;
  std::set<ComponentIndex> m_allComponentIndices;
  std::unordered_map<ComponentIndex, UUIDCompMap> m_store;

  const ComponentIndex newIndex(void) {
    ComponentIndex idx = nextIndex++;
    m_allComponentIndices.insert(idx);
    return idx;
  }

  template <typename TyComponent> const ComponentIndex index(void) {
    static const ComponentIndex idx = newIndex();
    return idx;
  }

  using SystemSet = std::set<System *>;
  using ComponentIndexSet = std::set<ComponentIndex>;
  std::unordered_map<ComponentIndex, SystemSet> m_subscribedSystems;
  std::unordered_map<System *, ComponentIndexSet> m_subscribedComponents;

  template <typename TyComponent> void bookComponent(const size_t alloc_size) {
    DEBUG("booking component: " << type_name<TyComponent>());
    const ComponentIndex newCompIDX = index<TyComponent>();

    // add component to store
    if (m_store.find(newCompIDX) != m_store.end()) {
      throw;
    } else {
      m_store.emplace(newCompIDX, alloc_size);
    }

    // add component to subscription map
    const auto newComp = m_store.find(newCompIDX);
    if (m_subscribedSystems.find(newCompIDX) != m_subscribedSystems.end()) {
      throw;
    } else {
      m_subscribedSystems[newCompIDX] = {};
    }
  }

  template <typename TyComponent>
  void alertSystemsNewComponentAdded(const UUID &uuid) {
    for (auto &sys : m_subscribedSystems.at(index<TyComponent>())) {
      if (sys->isFollowing(uuid))
        continue;

      bool entityShouldBeFollowed = true;

      for (const ComponentIndex idx : m_subscribedComponents.at(sys)) {
        if (m_store.at(idx).count(uuid.unwrap()) == 0) {
          entityShouldBeFollowed = false;
          break;
        }
      }

      if (entityShouldBeFollowed)
        sys->follow(uuid);
    }
  }

  template <typename TyComponent>
  void alertSystemsOldComponentRemoved(const UUID &uuid) {
    for (auto &sys : m_subscribedSystems.at(index<TyComponent>()))
      sys->unfollow(uuid);
  }

public:
  ComponentManager(const ComponentManager &) = delete;
  ComponentManager &operator=(const ComponentManager &) = delete;
  ComponentManager(ComponentManager &&) = delete;
  ComponentManager &operator=(ComponentManager &&) = delete;

  ComponentManager() {
    bookComponent<Position>(100);
    bookComponent<Velocity>(100);
    bookComponent<Sprite>(100);
  }
  ~ComponentManager() = default;

  template <typename TyComponent>
  void add(UUID uuid, const TyComponent &value) {
    // TODO: use templated args and make_unique, or allow pass pointer, or
    // rvalue  reference in order to speed this up (copying every time is
    // not sustainable)
    auto compID = index<TyComponent>();
    UUIDCompMap &compMap = m_store.at(compID);

    auto oldValue = compMap.find(uuid.unwrap());

    if (oldValue != compMap.end()) {
      // only want to call this method
      // to add a component to an entity that doesn't
      // already have it
      throw;
    } else {
      compMap.emplace(uuid.unwrap(),
                      static_cast<void *>(new TyComponent(value)));
      alertSystemsNewComponentAdded<TyComponent>(uuid);
    }

    DEBUG("added " << type_name<TyComponent>()
                   << " to entity with UUID: " << uuid);
  }

  template <typename TyComponent> void add(UUID uuid, TyComponent *value) {
    // TODO: use templated args and make_unique, or allow pass pointer, or
    // rvalue  reference in order to speed this up (copying every time is
    // not sustainable)
    auto compID = index<TyComponent>();
    UUIDCompMap &compMap = m_store.at(compID);

    auto oldValue = compMap.find(uuid.unwrap());

    if (oldValue != compMap.end()) {
      // only want to call this method
      // to add a component to an entity that doesn't
      // already have it
      throw;
    } else {
      compMap.emplace(uuid.unwrap(), static_cast<void *>(value));
      alertSystemsNewComponentAdded<TyComponent>(uuid);
    }

    DEBUG("added " << type_name<TyComponent>()
                   << " to entity with UUID: " << uuid);
  }

  template <typename TyComponent> void remove(const UUID &uuid) {
    auto compID = index<TyComponent>();
    UUIDCompMap &compMap = m_store.at(compID);

    auto oldValue = compMap.find(uuid.unwrap());

    if (oldValue == compMap.end()) {
      throw; // tried to erase component that doesn't exit
    } else {
      compMap.erase(uuid.unwrap());
      delete static_cast<TyComponent *>(oldValue->second);
      alertSystemsOldComponentRemoved<TyComponent>(uuid);
    }
  }

  template <typename TyComponent> TyComponent *get(const UUID &uuid) {
    UUIDCompMap &compMap = m_store.at(index<TyComponent>());

    auto comp = compMap.find(uuid.unwrap());

    if (comp != compMap.end()) {
      return static_cast<TyComponent *>(comp->second);
    } else {
      return nullptr;
    }
  }

  template <typename TyComponent> const TyComponent *get(UUID uuid) const {
    const UUIDCompMap &compMap = m_store.at(index<TyComponent>());

    const auto comp = compMap.find(uuid.unwrap());

    if (comp != compMap.end()) {
      return static_cast<const TyComponent *>(comp->second);
    } else {
      return nullptr;
    }
  }

  template <typename TyComponent> bool has(UUID uuid) {
    const UUIDCompMap &compMap = m_store.at(index<TyComponent>());
    return compMap.count(uuid.unwrap());
  }

  template <typename TyComponent> void subscribe(System *s) {
    auto idx = index<TyComponent>();
    if (m_subscribedComponents.find(s) == m_subscribedComponents.end()) {
      m_subscribedComponents[s] = {};
    }

    m_subscribedSystems.at(idx).insert(s);
    m_subscribedComponents.at(s).insert(idx);

    DEBUG(s->name << " system subscribed to: " << type_name<TyComponent>());
  }

  template <typename TyComponent1, typename TyComponent2,
            typename... TyComponents>
  void subscribe(System *s) {
    subscribe<TyComponent1>(s);
    subscribe<TyComponent2, TyComponents...>(s);
  }
};
