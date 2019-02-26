#pragma once

#include "System.hpp"
#include "UUID.hpp"

#include "Util.hpp"

#include "unstd/BucketArray.hpp"
#include "unstd/ArraySet.hpp"
#include "unstd/DenseHashTable.hpp"
#include "unstd/DynamicArray.hpp"

class ComponentManager {
    ////////////////////////
    // Component Indexing //
    ////////////////////////

    // A ComponentIndex is used to map an unsigned integer to a particular component type
    // for book-keeping purposes
    using ComponentIndex = u32;

    u32 m_destroyCount = 0;

    ComponentIndex m_nextIndex;

    template <typename T>
    inline ComponentIndex index(void) {
        static const ComponentIndex idx = newIndex();
        return idx;
    }

    inline ComponentIndex newIndex(void) {
        ComponentIndex idx = m_nextIndex;
        m_nextIndex++;
        return idx;
    }

    ////////////////////////////
    // Component Data Storage //
    ////////////////////////////

    DynamicArray<DenseHashTable<UUID::rep, BucketIndex>> component_handles;
    DynamicArray<BucketArrayBase> component_storage;

    /////////////////////////////////////////
    // System <--> Component Subscriptions //
    /////////////////////////////////////////

    using SystemSet = ArraySet<System*>;
    DenseHashTable<ComponentIndex, SystemSet> m_subscribedSystems;
    DenseHashTable<System*, ArraySet<ComponentIndex>> m_subscribedComponents;
    DynamicArray<System*> m_allSystemsArray;

    // DenseHashTable<ComponentIndex, DynamicArray<UUID>> m_deleteComponentQueue;
    // DynamicArray<UUID>> m_destroyQueue;

    template <typename T>
    inline bool componentIsRegistered(void) {
        const ComponentIndex compID = index<T>();
        return compID < component_handles.size() && compID < component_storage.size();
    }

public:
    ComponentManager()
        : m_nextIndex(0)
        , m_subscribedComponents(DenseHashTable<System*, ArraySet<ComponentIndex>>::create(64, reinterpret_cast<System*>(1), reinterpret_cast<System*>(2)))
        , m_subscribedSystems(DenseHashTable<ComponentIndex, SystemSet>::create(64, 0xDEADBEEF, 0xFEEDFACE))
    {}

    ~ComponentManager() {
        for (auto& m : component_handles)
            m.destroy();
    }

    u32 alive_count(void) const {
        return UUID::m_nextID - m_destroyCount;
    };

    void tick(f32 dt) {
        for (System* sys : m_allSystemsArray) {
            if (sys->active) {
                sys->run(this, dt);
            }
        }
    }

    template <typename T>
    void registerComponent(const u16 bucket_size);

    template <typename T>
    inline void unRegisterComponent(void);

    template <typename T, class... Args>
    T& book(const UUID& uuid, Args&&... args);

    template <typename T>
    void remove(const UUID& uuid);

    template <typename T>
    inline T& get(const UUID& uuid);

    inline void destroy(const UUID& uuid);

    template <typename T>
    inline bool has(UUID uuid) {
        const ComponentIndex compID = index<T>();
        return component_handles[compID].contains(uuid.unwrap());
    }

    template <typename T>
    void subscribe(System* sys);

    template <typename T1, typename... Types>
    void subscribe(const TypeList<T1, Types...>&, System* s) {
        subscribe<T1>(s);
        subscribe<Types...>(TypeList<Types...>(), s);
    }

    template <typename... Types>
    void subscribe(const TypeList<>&, System*) {
        return;
    }

    template <typename TySystem>
    void registerSystem(TySystem& sys) {
        m_allSystemsArray.append(&sys);
        subscribe(TySystem::subscriptions, &sys);
    }
};

template <typename T>
void ComponentManager::registerComponent(const u16 bucket_size) {
    const ComponentIndex newCompID = index<T>();
    DEBUG("registering component: " << type_name<T>() << " with new index: " << newCompID);

    assert(component_handles.size() == newCompID &&
           "Expected/Actual mismatch between number of registered component stores when registering ");
    assert(component_storage.size() == newCompID &&
           "Expected/Actual mismatch between number of registered component pools when registering ");

    // add component to store
    component_handles.append(DenseHashTable<UUID::rep, BucketIndex>::create(256, 0, 1));

    // create memory pool for component
    component_storage.append(static_cast<BucketArrayBase>(BucketArray<T>::create(bucket_size)));

    // add component to subscription map (with no subscriptions yet)
    m_subscribedSystems.insert(newCompID, SystemSet());
}

template <typename T>
inline void ComponentManager::unRegisterComponent(void) {
    DEBUG("un-registering component: " << type_name<T>());
    // TODO: remove from component_handles too
    const ComponentIndex compID = index<T>();
    BucketArrayBase::destroy(&component_storage[compID]);
}

template <typename T, class... Args>
T& ComponentManager::book(const UUID& uuid, Args&&... args) {
    const ComponentIndex compID = index<T>();

    DEBUG(uuid << " booked component: " << type_name<T>());

    assert(!component_handles[compID].contains(uuid.unwrap()) &&
          "Attempted to book component for entity that already posseses it");

    BucketArray<T>& arr = static_cast<BucketArray<T>&>(component_storage[compID]);
    const BucketIndex handle = insert(&arr, args...);
    component_handles[compID].insert(uuid.unwrap(), handle);

    for (auto& sys : *m_subscribedSystems.lookup(index<T>())) {
        if (sys->is_following(uuid))
            continue;

        bool entityShouldBeFollowed = true;

        for (const ComponentIndex idx : *m_subscribedComponents.lookup(sys)) {
            if (!component_handles[idx].contains(uuid.unwrap())) {
                entityShouldBeFollowed = false;
                break;
            }
        }

        if (entityShouldBeFollowed)
            sys->follow(uuid);
    }

    return arr[handle];
}

template <typename T>
void ComponentManager::remove(const UUID& uuid) {
    const ComponentIndex compID = index<T>();

    DEBUG(uuid << " removed component: " << type_name<T>());

    const BucketIndex* oldHandlePtr = component_handles[compID].lookup(uuid.unwrap());
    assert(oldHandlePtr != nullptr && "Attempted to remove non-existent component from entity");
    const BucketIndex oldHandle = *oldHandlePtr;

    component_handles[compID].remove(uuid.unwrap());

    remove_item(&component_storage[compID], oldHandle);

    for (auto& sys : *m_subscribedSystems.lookup(index<T>()))
        sys->unfollow(uuid);
}

template <typename T>
inline T& ComponentManager::get(const UUID& uuid) {
    assert(componentIsRegistered<T>() &&
           "Attempted to 'get' component data for un-registered component of type: ");

    const ComponentIndex compID = index<T>();

    const BucketIndex* oldHandlePtr = component_handles[compID].lookup(uuid.unwrap());
    assert(oldHandlePtr != nullptr && "Attempted to access non-existent component for entity");

	BucketArray<T>& arr = static_cast<BucketArray<T>&>(component_storage[compID]);
    return arr[*oldHandlePtr];
}

void ComponentManager::destroy(const UUID& uuid) {
    DEBUG("Destroying UUID: " << uuid);

    for (System* sys : m_allSystemsArray) {
        sys->unfollow(uuid);
    }

    for (auto idx = 0; idx < m_nextIndex; idx++) {
        const BucketIndex* oldHandlePtr = component_handles[idx].lookup(uuid.unwrap());
        if (oldHandlePtr != nullptr) {
            remove_item(&component_storage[idx], *oldHandlePtr);
            component_handles[idx].remove(uuid.unwrap());
        }
    }

    m_destroyCount++;
}

template <typename T>
void ComponentManager::subscribe(System* sys) {
    const ComponentIndex idx = index<T>();

    if (m_subscribedComponents.lookup(sys) == nullptr) {
        m_subscribedComponents.insert(sys, ArraySet<ComponentIndex>());
    }

    m_subscribedSystems[idx].insert(sys);
    m_subscribedComponents[sys].insert(idx);

    DEBUG(sys->name << " system subscribed to: " << type_name<T>());
}

