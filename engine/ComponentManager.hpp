#pragma once

#include "System.hpp"
#include "UUID.hpp"

#include "Util.hpp"

#include <fstream>
#include <iostream>
#include <istream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "BucketArray.hpp"
#include "unstd/ArraySet.hpp"
#include "unstd/DenseHashTable.hpp"
#include "unstd/DynamicArray.hpp"

class ComponentManager {
private:
    ////////////////////////
    // Component Indexing //
    ////////////////////////

    // A ComponentIndex is used to map an unsigned integer to a particular component type
    // for book-keeping purposes
    using ComponentIndex = u32;
    using ComponentIndexSet = ArraySet<ComponentIndex>;

    DynamicArray<ComponentIndex> m_allComponentIndices;
    ComponentIndex m_nextIndex;

    template <typename TyComponent>
    ComponentIndex index(void) {
        static const ComponentIndex idx = newIndex();
        return idx;
    }

    inline ComponentIndex newIndex(void) {
        ComponentIndex idx = m_nextIndex;
        m_nextIndex++;
        append(&m_allComponentIndices, idx);
        return idx;
    }

    ////////////////////////////
    // Component Data Storage //
    ////////////////////////////

    using UUIDCompMap = DenseHashTable<UUID::rep, BucketIndex>;
    DynamicArray<UUIDCompMap> m_store;
    DynamicArray<BucketArrayBase> m_pools;

    /////////////////////////////////////////
    // System <--> Component Subscriptions //
    /////////////////////////////////////////

    using SystemSet = std::unordered_set<System*>;
    std::unordered_map<ComponentIndex, SystemSet> m_subscribedSystems;
    DenseHashTable<System*, ComponentIndexSet> m_subscribedComponents;
    ArraySet<System*> m_allSystems;

    template <typename TyComponent>
    void alertSystemsNewComponentAdded(const UUID& uuid);

    template <typename TyComponent>
    inline void alertSystemsOldComponentRemoved(const UUID& uuid);

    template <typename TyComponent>
    inline bool componentIsRegistered(void) {
        const ComponentIndex compID = index<TyComponent>();
        return compID < m_store.size && compID < m_pools.size;
    }

public:
    ComponentManager(const ComponentManager&) = delete;
    ComponentManager& operator=(const ComponentManager&) = delete;
    ComponentManager(ComponentManager&&) = delete;
    ComponentManager& operator=(ComponentManager&&) = delete;

    ComponentManager()
        : m_nextIndex(0)
        , m_subscribedComponents(DenseHashTable<System*, ComponentIndexSet>::create(100, reinterpret_cast<System*>(1), reinterpret_cast<System*>(2)))
    {}

    ~ComponentManager() {
        for (UUIDCompMap& m : m_store)
            UUIDCompMap::destroy(&m);
    }

    template <typename TyComponent>
    void registerComponent(const u16 bucket_size);

    template <typename TyComponent>
    inline void unRegisterComponent(void);

    template <typename TyComponent, class... Args>
    TyComponent& book(const UUID& uuid, Args&&... args);

    template <typename TyComponent>
    void remove(const UUID& uuid);

    template <typename TyComponent>
    inline TyComponent& get(const UUID& uuid);

    template <typename TyComponent>
    inline const TyComponent& get(UUID uuid) const;

    inline void destroy(const UUID& uuid);

    template <typename TyComponent>
    inline bool has(UUID uuid) {
        const ComponentIndex compID = index<TyComponent>();
        return contains(&m_store[compID], uuid.unwrap());
    }

    // TODO: don't need multiple subscribe definitions, use parameter pack tools
    template <typename TyComponent>
    void subscribe(System* sys);

    template <typename TyComponent1, typename TyComponent2, typename... TyComponents>
    void subscribe(System* s) {
        subscribe<TyComponent1>(s);
        subscribe<TyComponent2, TyComponents...>(s);
    }

    template <typename T1, typename... Types>
    void subscribe(const TypeList<T1, Types...>&, System* s) {
        subscribe<T1>(s);
        subscribe<Types...>(TypeList<Types...>(), s);
    }

    template <typename... Types>
    void subscribe(const TypeList<>&, System*) {
        return;
    }

    // TODO: unsubscribe? or just a disconnect system method
};

// TODO: pass lambda that runs on component removal?
template <typename TyComponent>
void ComponentManager::registerComponent(const u16 bucket_size) {
    // static_assert(std::is_standard_layout<TyComponent>::value, "Component Type must have standard layout.");
    // static_assert(std::is_trivially_copyable<TyComponent>::value, "Component Type must be trivally copyable.");

    const ComponentIndex newCompID = index<TyComponent>();
    DEBUG("registering component: " << type_name<TyComponent>() << " with new index: " << newCompID);

    assert(m_store.size == newCompID &&
           "Expected/Actual mismatch between number of registered component stores when registering ");
    assert(m_pools.size == newCompID &&
           "Expected/Actual mismatch between number of registered component pools when registering ");

    // add component to store
    append(&m_store, UUIDCompMap::create(bucket_size, 0, 1));

    // create memory pool for component
    append(&m_pools, static_cast<BucketArrayBase>(BucketArray<TyComponent>::create(bucket_size)));

    // debug_print(&m_store);

    // add component to subscription map (with no subscriptions yet)
    m_subscribedSystems.emplace(newCompID, SystemSet());
}

template <typename TyComponent>
inline void ComponentManager::unRegisterComponent(void) {
    DEBUG("un-registering component: " << type_name<TyComponent>());
    // TODO: remove from UUIDCompMaps too
    const ComponentIndex compID = index<TyComponent>();
    BucketArrayBase::destroy(&m_pools[compID]);
}

template <typename TyComponent>
// OPTIMIZE: defer this check when building entities?
// So far, entity creation/deletion has no real impact on performance though
void ComponentManager::alertSystemsNewComponentAdded(const UUID& uuid) {
    for (auto& sys : m_subscribedSystems.at(index<TyComponent>())) {
        if (sys->is_following(uuid))
            continue;

        bool entityShouldBeFollowed = true;

        for (const ComponentIndex idx : *lookup(&m_subscribedComponents, sys)) {
            if (!contains(&m_store[idx], uuid.unwrap())) {
                entityShouldBeFollowed = false;
                break;
            }
        }

        if (entityShouldBeFollowed)
            sys->follow(uuid);
    }
}

template <typename TyComponent>
inline void ComponentManager::alertSystemsOldComponentRemoved(const UUID& uuid) {
    for (auto& sys : m_subscribedSystems.at(index<TyComponent>()))
        sys->unfollow(uuid);
}

template <typename TyComponent, class... Args>
TyComponent& ComponentManager::book(const UUID& uuid, Args&&... args) {
    const ComponentIndex compID = index<TyComponent>();

    DEBUG(uuid << " booked component: " << type_name<TyComponent>());

    // assert(!contains(compMap, uuid.unwrap()) &&
    //       "Attempted to book component for entity that already posseses it");

    assert(compID < m_pools.size);

    BucketArray<TyComponent>& arr = static_cast<BucketArray<TyComponent>&>(m_pools[compID]);
    const BucketIndex handle = insert(&arr, args...);
    insert(&m_store[compID], uuid.unwrap(), handle);
    alertSystemsNewComponentAdded<TyComponent>(uuid);

    return arr[handle];
}

template <typename TyComponent>
void ComponentManager::remove(const UUID& uuid) {
    const ComponentIndex compID = index<TyComponent>();

    DEBUG(uuid << " removed component: " << type_name<TyComponent>());

    const BucketIndex* oldHandlePtr = lookup(&m_store[compID], uuid.unwrap());
    assert(oldHandlePtr != nullptr && "Attempted to remove non-existent component from entity");
    const BucketIndex oldHandle = *oldHandlePtr;

    ::remove(&m_store[compID], uuid.unwrap());

    remove_item(&m_pools[compID], oldHandle);

    for (auto& sys : m_subscribedSystems.at(index<TyComponent>()))
        sys->unfollow(uuid);
}

template <typename TyComponent>
inline TyComponent& ComponentManager::get(const UUID& uuid) {
    assert(componentIsRegistered<TyComponent>() &&
           "Attempted to 'get' component data for un-registered component of type: ");

    const ComponentIndex compID = index<TyComponent>();

    const BucketIndex* oldHandlePtr = lookup(&m_store[compID], uuid.unwrap());
    assert(oldHandlePtr != nullptr && "Attempted to access non-existent component for entity");

	BucketArray<TyComponent>& arr = static_cast<BucketArray<TyComponent>&>(m_pools[compID]);
    return arr[*oldHandlePtr];
}

template <typename TyComponent>
inline const TyComponent& ComponentManager::get(UUID uuid) const {
    const ComponentIndex compID = index<TyComponent>();

    assert(compID < m_store.size && compID < m_pools.size &&
           "Attempted to 'get' component data for un-registered component of type: ");

    const BucketIndex* oldHandlePtr = lookup(&m_store[compID], uuid.unwrap());
    assert(oldHandlePtr != nullptr && "Attempted to access non-existent component for entity");
    const auto arr = static_cast<const BucketArray<TyComponent>>(m_pools[compID]);
    return arr[*oldHandlePtr];
}

void ComponentManager::destroy(const UUID& uuid) {
    DEBUG("Destroying UUID: " << uuid);

    for (System* sys : m_allSystems) {
        sys->unfollow(uuid);
    }

    for (const auto& idx : m_allComponentIndices) {
        const BucketIndex* oldHandlePtr = lookup(&m_store[idx], uuid.unwrap());
        if (oldHandlePtr != nullptr) {
            remove_item(&m_pools[idx], *oldHandlePtr);
            ::remove(&m_store[idx], uuid.unwrap());
        }
    }
}

template <typename TyComponent>
void ComponentManager::subscribe(System* sys) {
    insert(m_allSystems, sys);

    const ComponentIndex idx = index<TyComponent>();

    if (lookup(&m_subscribedComponents, sys) == nullptr) {
        insert(&m_subscribedComponents, sys, ComponentIndexSet());
    }

    m_subscribedSystems[idx].insert(sys);
    insert(m_subscribedComponents[sys], idx);

    DEBUG(sys->name << " system subscribed to: " << type_name<TyComponent>());
    // debug_print(m_subscribedComponents[sys]);
}

