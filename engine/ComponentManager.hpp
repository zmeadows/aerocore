#pragma once

#include "System.hpp"
#include "UUID.hpp"

#include "Util.hpp"

#include <fstream>
#include <iostream>
#include <istream>

#include "BucketArray.hpp"
#include "unstd/ArraySet.hpp"
#include "unstd/DenseHashTable.hpp"
#include "unstd/DynamicArray.hpp"
#include "unstd/DynamicBitSet.hpp"

struct ComponentManager {
    ////////////////////////
    // Component Indexing //
    ////////////////////////

    using ComponentIndex = u32; // maps component type type to integer for book-keeping

    ComponentIndex next_index;
    DynamicArray<ComponentIndex> all_component_indices;

    template <typename T>
    ComponentIndex index(void) {
        static const ComponentIndex idx = new_index();
        return idx;
    }

    ComponentIndex new_index(void) {
        ComponentIndex idx = next_index;
        next_index++;
        append(&all_component_indices, idx);
        return idx;
    }

    ////////////////////////////
    // Component Data Storage //
    ////////////////////////////

    using EntityToComponentHandleMap = DenseHashTable<UUID::rep, BucketIndex>;
    DynamicArray<EntityToComponentHandleMap*> entity_handle_maps;
    DynamicArray<void*> component_storage;

    /////////////////////////////////////////
    // System <--> Component Subscriptions //
    /////////////////////////////////////////

    ArraySet<System*> m_allSystems;
    DenseHashTable<UUID::rep, DynamicBitSet> entity_component_masks;
    DenseHashTable<System*, DynamicBitSet> system_component_masks;

    template <typename T>
    bool componentIsRegistered(void) {
        ComponentIndex compID = index<T>();
        return compID < entity_handle_maps.size && compID < component_storage.size;
    }

    ComponentManager(const ComponentManager&) = delete;
    ComponentManager& operator=(const ComponentManager&) = delete;
    ComponentManager(ComponentManager&&) = delete;
    ComponentManager& operator=(ComponentManager&&) = delete;

    ComponentManager()
        : next_index(0), entity_component_masks(DenseHashTable<UUID::rep, DynamicBitSet>::create(1000, 0, 1)),
          system_component_masks(DenseHashTable<System*, DynamicBitSet>::create(
              1000, reinterpret_cast<System*>(1), reinterpret_cast<System*>(2))) {}

    ~ComponentManager() {
        for (EntityToComponentHandleMap* m : entity_handle_maps)
            EntityToComponentHandleMap::destroy(m);
    }

    template <typename T>
    void registerComponent(const u16 bucket_size);

    template <typename T>
    void unRegisterComponent(void);

    template <typename T, class... Args>
    T& book(const UUID& uuid, Args&&... args);

    template <typename T>
    void remove(const UUID& uuid);

    template <typename T>
    T& get(const UUID& uuid);

    template <typename T>
    const T& get(UUID uuid) const;

    void destroy(const UUID& uuid) {
        DEBUG("Destroying UUID: " << uuid);

        for (System* sys : m_allSystems) {
            sys->unfollow(uuid);
        }

        ::remove(&entity_component_masks, uuid.unwrap());

        for (const auto& idx : all_component_indices) {
            const BucketIndex* oldHandlePtr = lookup(entity_handle_maps[idx], uuid.unwrap());
            if (oldHandlePtr != nullptr) {
                auto arr = static_cast<BucketArrayBase*>(component_storage[idx]);
                remove_item(arr, *oldHandlePtr);
                ::remove(entity_handle_maps[idx], uuid.unwrap());
            }
        }
    }

    template <typename T>
    bool has(UUID uuid) {
        const ComponentIndex compID = index<T>();
        // TODO: just check the component mask
        return contains(entity_handle_maps[compID], uuid.unwrap());
    }

    // TODO: don't need multiple subscribe definitions, use parameter pack tools
    template <typename T>
    void subscribe(System* sys);

    template <typename T1, typename T2, typename... Ts>
    void subscribe(System* s) {
        subscribe<T1>(s);
        subscribe<T2, Ts...>(s);
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

    // TODO: add a method to disconnect a system and remove all traces of it
    // this will be useful for turning on/off debug systems
};

// TODO: pass lambda that runs on component removal?
template <typename T>
void ComponentManager::registerComponent(const u16 bucket_size) {
    // static_assert(std::is_standard_layout<T>::value, "Component Type must have standard layout.");
    // static_assert(std::is_trivially_copyable<T>::value, "Component Type must be trivally copyable.");

    const ComponentIndex newCompID = index<T>();
    DEBUG("registering component: " << type_name<T>() << " with new index: " << newCompID);

    assert(entity_handle_maps.size == newCompID &&
           "Expected/Actual mismatch between number of registered component stores when registering ");
    assert(component_storage.size == newCompID &&
           "Expected/Actual mismatch between number of registered component pools when registering ");

    auto new_map = memalloc<EntityToComponentHandleMap>(1);
    *new_map = EntityToComponentHandleMap::create(bucket_size, 0, 1);
    debug_print(new_map);
    // add component to store
    append(&entity_handle_maps, new_map);

    // create memory pool for component
    BucketArray<T>* arr = BucketArray<T>::allocate(bucket_size);
    append(&component_storage, static_cast<void*>(arr));
}

template <typename T>
void ComponentManager::unRegisterComponent(void) {
    DEBUG("un-registering component: " << type_name<T>());
    // TODO: remove from EntityToComponentHandleMaps too
    const ComponentIndex compID = index<T>();
    auto arr = static_cast<BucketArrayBase*>(component_storage[compID]);
    BucketArrayBase::destroy(arr);
}

template <typename T, class... Args>
T& ComponentManager::book(const UUID& uuid, Args&&... args) {
    const ComponentIndex comp_index = index<T>();

    DEBUG(uuid << " booked component: " << type_name<T>() << " with index " << comp_index);

    // TOOD: write lookup_or_create function?
    DynamicBitSet* component_mask = lookup(&entity_component_masks, uuid.unwrap());

    if (component_mask == nullptr) {
        component_mask = insert(&entity_component_masks, uuid.unwrap(), DynamicBitSet());
    }

    set_bit(component_mask, comp_index);
    debug_print(component_mask);

    EntityToComponentHandleMap* compMap = entity_handle_maps[comp_index];

    assert(!contains(compMap, uuid.unwrap()) &&
           "Attempted to book component for entity that already posseses it");

    assert(comp_index < component_storage.size);

    auto arr = static_cast<BucketArray<T>*>(component_storage[comp_index]);
    const BucketIndex handle = insert(arr, args...);
    DEBUG("\t bucket index --> " << handle);
    insert(compMap, uuid.unwrap(), handle);

    for (auto& sys : m_allSystems) {
        DynamicBitSet* system_mask = lookup(&system_component_masks, sys);
        if (is_subset_of(system_mask, component_mask)) {
            sys->follow(uuid);
        }
    }

    assert(has<T>(uuid));

    return (*arr)[handle];
}

template <typename T>
void ComponentManager::remove(const UUID& uuid) {
    const ComponentIndex compID = index<T>();

    DEBUG(uuid << " removed component: " << type_name<T>());

    const BucketIndex* oldHandlePtr = lookup(entity_handle_maps[compID], uuid.unwrap());
    assert(oldHandlePtr != nullptr && "Attempted to remove non-existent component from entity");
    const BucketIndex oldHandle = *oldHandlePtr;

    ::remove(entity_handle_maps[compID], uuid.unwrap());

    auto arr = static_cast<BucketArrayBase*>(component_storage[compID]);
    remove_item(arr, oldHandle);

    ComponentIndex comp_index = index<T>();
    for (System* sys : m_allSystems) {
        DynamicBitSet* system_mask = lookup(&system_component_masks, sys);
        assert(system_mask && "System missing a component mask!");
        if (check_bit(system_mask, comp_index)) {
            sys->unfollow(uuid);
        }
    }
}

template <typename T>
T& ComponentManager::get(const UUID& uuid) {
    assert(componentIsRegistered<T>() && "Attempted to 'get' component data for un-registered component");

    DEBUG(uuid << " attempting to access component " << type_name<T>());

    const ComponentIndex compID = index<T>();

    const BucketIndex* oldHandlePtr = lookup(entity_handle_maps[compID], uuid.unwrap());
    assert(oldHandlePtr != nullptr && "Attempted to access non-existent component for entity");

    auto arr = static_cast<BucketArray<T>*>(component_storage[compID]);
    return (*arr)[*oldHandlePtr];
}

template <typename T>
const T& ComponentManager::get(UUID uuid) const {
    const ComponentIndex compID = index<T>();

    assert(componentIsRegistered<T>() && "Attempted to 'get' component data for un-registered component");

    const EntityToComponentHandleMap& compMap = *(entity_handle_maps[compID]);

    const BucketIndex* oldHandlePtr = lookup(compMap, uuid.unwrap());
    assert(oldHandlePtr != nullptr && "Attempted to access non-existent component for entity");
    const auto arr = static_cast<const BucketArray<T>*>(component_storage[compID]);
    return arr[*oldHandlePtr];
}

template <typename T>
void ComponentManager::subscribe(System* sys) {
    insert(m_allSystems, sys);

    const ComponentIndex comp_idx = index<T>();

    if (lookup(&system_component_masks, sys) == nullptr) {
        insert(&system_component_masks, sys, DynamicBitSet());
    }

    DynamicBitSet* system_mask = lookup(&system_component_masks, sys);
    assert(system_mask, "Failed to create component mask for system!");
    set_bit(system_mask, comp_idx);

    DEBUG(sys->name << " system subscribed to: " << type_name<T>());
}
