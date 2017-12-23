#pragma once

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Base.hpp"
#include "BoundingSurface.hpp"
#include "ResourceManager.hpp"
#include "Sprite.hpp"
#include "System.hpp"
#include "UUID.hpp"
#include "Util.hpp"

class ComponentManager {
private:
    ////////////////////////
    // Component Indexing //
    ////////////////////////

    // A ComponentIndex is used to map an unsigned integer to a type
    // for the purposes of keeping track of generic data in aggregate structures
    using ComponentIndex = size_t;
    using ComponentIndexSet = std::unordered_set<ComponentIndex>;

    ComponentIndexSet m_allComponentIndices;
    ComponentIndex m_nextIndex;

    template <typename TyComponent>
    ComponentIndex index(void) {
        static const ComponentIndex idx = newIndex();
        return idx;
    }

    const ComponentIndex newIndex(void) {
        ComponentIndex idx = m_nextIndex;
        m_nextIndex++;
        m_allComponentIndices.insert(idx);
        return idx;
    }

    ////////////////////////////
    // Component Data Storage //
    ////////////////////////////

    // TODO: switch to vector of handles, with size == max number of entities
    // will be a little tough and bug-prone to implement, since we would have to re-use UUIDs
    // and implement a UUID pool
    using UUIDCompMap = std::unordered_map<UUID::rep, ResourceManager::Handle>;
    std::vector<UUIDCompMap> m_store;
    std::vector<std::unique_ptr<ResourceManager>> m_pools;

    /////////////////////////////////////////
    // System <--> Component Subscriptions //
    /////////////////////////////////////////

    // note: ComponentManager does not own the Systems, SystemManager does.
    using SystemSet = std::unordered_set<System*>;
    std::unordered_map<ComponentIndex, SystemSet> m_subscribedSystems;
    std::unordered_map<System*, ComponentIndexSet> m_subscribedComponents;
    std::unordered_set<System*> m_allSystems;

    template <typename TyComponent>
    void alertSystemsNewComponentAdded(const UUID& uuid);

    template <typename TyComponent>
    void alertSystemsOldComponentRemoved(const UUID& uuid);

    template <typename TyComponent>
    bool componentIsRegistered(void) {
        const ComponentIndex compID = index<TyComponent>();
        return compID < m_store.size() && compID < m_pools.size();
    }

public:
    ComponentManager(const ComponentManager&) = delete;
    ComponentManager& operator=(const ComponentManager&) = delete;
    ComponentManager(ComponentManager&&) = delete;
    ComponentManager& operator=(ComponentManager&&) = delete;

    ComponentManager() {
        m_pools.reserve(1000);
        m_store.reserve(1000);
        registerComponent<Acceleration>(50000);
        registerComponent<Alliance>(50000);
        registerComponent<BoundingSurface>(50000);
        registerComponent<ShotDelay>(50000);
        registerComponent<OffscreenBehavior>(50000);
        registerComponent<Position>(50000);
        registerComponent<Rotation>(50000);
        registerComponent<RotationalVelocity>(50000);
        registerComponent<Sprite>(50000);
        registerComponent<Velocity>(50000);
        registerComponent<ParticleGenerator>(50000);
        registerComponent<DeathTimer>(50000);
        registerComponent<SpriteUpdator>(50000);
    }

    ~ComponentManager() = default;

    template <typename TyComponent>
    void registerComponent(const size_t allocSize);

    // retrieve an allocated memory block to 'placement new' a component
    template <typename TyComponent>
    TyComponent* book(const UUID& uuid);

    template <typename TyComponent>
    void remove(const UUID& uuid);

    template <typename TyComponent>
    TyComponent* get(const UUID& uuid);

    template <typename TyComponent>
    const TyComponent* get(UUID uuid) const;

    inline void destroy(const UUID& uuid);

    template <typename TyComponent>
    bool has(UUID uuid) {
        const ComponentIndex compID = index<TyComponent>();
        return static_cast<bool>(m_store[compID].count(uuid.unwrap()));
    }

    template <typename TyComponent>
    void subscribe(System* sys);

    template <typename TyComponent1, typename TyComponent2, typename... TyComponents>
    void subscribe(System* s) {
        subscribe<TyComponent1>(s);
        subscribe<TyComponent2, TyComponents...>(s);
    }
};

template <typename TyComponent>
void ComponentManager::registerComponent(const size_t allocSize) {
    DEBUG("registering component: " << type_name<TyComponent>());

    const ComponentIndex newCompID = index<TyComponent>();

    DEBUG_ASSERT(m_store.size() == newCompID,
                 "Expected/Actual mismatch between number of registered component stores when registering "
                     << type_name<TyComponent>());

    DEBUG_ASSERT(m_pools.size() == newCompID,
                 "Expected/Actual mismatch between number of registered component pools when registering "
                     << type_name<TyComponent>());

    // add component to store
    m_store.emplace_back(UUIDCompMap());

    // create memory pool for component
    ResourceManager* rm = new ResourceManager();
    rm->allocate<TyComponent>(allocSize);
    m_pools.emplace_back(rm);

    // add component to subscription map (with no subscriptions yet)
    m_subscribedSystems.emplace(newCompID, SystemSet());
}

template <typename TyComponent>
void ComponentManager::alertSystemsNewComponentAdded(const UUID& uuid) {
    for (auto& sys : m_subscribedSystems.at(index<TyComponent>())) {
        if (sys->isFollowing(uuid))
            continue;

        bool entityShouldBeFollowed = true;

        for (const ComponentIndex idx : m_subscribedComponents.at(sys)) {
            if (m_store[idx].count(uuid.unwrap()) == 0) {
                entityShouldBeFollowed = false;
                break;
            }
        }

        if (entityShouldBeFollowed)
            sys->follow(uuid);
    }
}

template <typename TyComponent>
void ComponentManager::alertSystemsOldComponentRemoved(const UUID& uuid) {
    for (auto& sys : m_subscribedSystems.at(index<TyComponent>()))
        sys->unfollow(uuid);
}

template <typename TyComponent>
TyComponent* ComponentManager::book(const UUID& uuid) {
    const ComponentIndex compID = index<TyComponent>();

    UUIDCompMap& compMap = m_store[compID];

    DEBUG_ASSERT(!contains(compMap, uuid.unwrap()), "Attempted to book component of type "
                                                        << type_name<TyComponent>()
                                                        << " for entity that already posseses it");

    const ResourceManager::Handle handle = m_pools[compID]->reserve();
    compMap[uuid.unwrap()] = handle;

    DEBUG("added " << type_name<TyComponent>() << " component to entity with UUID: " << uuid);

    return m_pools[compID]->get<TyComponent>(handle);
}

template <typename TyComponent>
void ComponentManager::remove(const UUID& uuid) {
    const ComponentIndex compID = index<TyComponent>();

    UUIDCompMap& compMap = m_store[compID];
    auto oldValue = compMap.find(uuid.unwrap());
    DEBUG_ASSERT(oldValue != compMap.end(),
                 "Attempted to remove non-existent component for entity with UUID" << uuid.unwrap());
    const ResourceManager::Handle oldHandle = oldValue->second;

    compMap.erase(oldValue);

    m_pools[compID]->release<TyComponent>(oldHandle);

    alertSystemsOldComponentRemoved<TyComponent>(uuid);
}

template <typename TyComponent>
TyComponent* ComponentManager::get(const UUID& uuid) {
    DEBUG_ASSERT(componentIsRegistered<TyComponent>(),
                 "Attempted to 'get' component data for un-registered component of type: "
                     << type_name<TyComponent>());

    const ComponentIndex compID = index<TyComponent>();

    UUIDCompMap& compMap = m_store[compID];

    auto hit = compMap.find(uuid.unwrap());
    if (hit != compMap.end()) {
        return m_pools[compID]->get<TyComponent>(hit->second);
    } else {
        return nullptr;
    }
}

template <typename TyComponent>
const TyComponent* ComponentManager::get(UUID uuid) const {
    const ComponentIndex compID = index<TyComponent>();

    DEBUG_ASSERT(compID < m_store.size() && compID < m_pools.size(),
                 "Attempted to 'get' component data for un-registered component of type: "
                     << type_name<TyComponent>());

    const UUIDCompMap& compMap = m_store.at(compID);

    const auto hit = compMap.find(uuid.unwrap());

    if (hit != compMap.end()) {
        const void* ptr = m_pools.at(compID)->get<TyComponent>(hit->second);
        return static_cast<const TyComponent*>(ptr);
    } else {
        return nullptr;
    }
}

void ComponentManager::destroy(const UUID& uuid) {
    DEBUG("Destroying UUID: " << uuid);

    for (auto& p : m_subscribedComponents) {
        System* sys = p.first;
        sys->unfollow(uuid);
    }

    // TODO: Need to allow user to pass custom destroy function...
    for (const auto& idx : m_allComponentIndices) {
        m_store[idx].erase(uuid.unwrap());
    }
}

template <typename TyComponent>
void ComponentManager::subscribe(System* sys) {
    const ComponentIndex idx = index<TyComponent>();

    if (m_subscribedComponents.find(sys) == m_subscribedComponents.end()) {
        m_subscribedComponents[sys] = {};
    }

    m_subscribedSystems[idx].insert(sys);
    m_subscribedComponents[sys].insert(idx);

    DEBUG(sys->name << " system subscribed to: " << type_name<TyComponent>());
}
