#pragma once

#include <cstdint>
#include <memory>
#include <set>
#include <unordered_map>
#include <vector>

#include "Base.hpp"
#include "BoundingSurface.hpp"
#include "Sprite.hpp"
#include "System.hpp"
#include "UUID.hpp"
#include "Util.hpp"

struct ComponentData {
    virtual ~ComponentData() {}
};

class ComponentManager {
private:
    using ComponentPtr = std::unique_ptr<void, void (*)(void*)>;
    using UUIDCompMap = std::unordered_map<UUID::rep, ComponentPtr>;
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

    using SystemSet = std::set<System*>;
    using ComponentIndexSet = std::set<ComponentIndex>;
    std::unordered_map<ComponentIndex, SystemSet> m_subscribedSystems;
    std::unordered_map<System*, ComponentIndexSet> m_subscribedComponents;

    template <typename TyComponent>
    void bookComponent(const size_t alloc_size) {
        DEBUG("booking component: " << type_name<TyComponent>());
        const ComponentIndex newCompIDX = index<TyComponent>();

        // add component to store
        if (m_store.find(newCompIDX) != m_store.end()) {
            throw;
        } else {
            m_store.emplace(newCompIDX, alloc_size);
        }

        // add component to subscription map
        // const auto newComp = m_store.find(newCompIDX);
        if (m_subscribedSystems.find(newCompIDX) != m_subscribedSystems.end()) {

            throw;
        } else {
            m_subscribedSystems[newCompIDX] = {};
        }
    }

    template <typename TyComponent>
    void alertSystemsNewComponentAdded(const UUID& uuid) {
        for (auto& sys : m_subscribedSystems.at(index<TyComponent>())) {
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
    void alertSystemsOldComponentRemoved(const UUID& uuid) {
        for (auto& sys : m_subscribedSystems.at(index<TyComponent>()))
            sys->unfollow(uuid);
    }

    template <typename TyComponent> UUIDCompMap& getCompMap(void) {
        auto compID = index<TyComponent>();
        assert(m_store.find(compID) != m_store.end());
        return m_store[compID];
    }

    template <typename TyComponent>
    void insert(const UUID& uuid, TyComponent* data) {
        UUIDCompMap& compMap = getCompMap<TyComponent>();

        assert(compMap.find(uuid.unwrap()) == compMap.end());

        compMap.emplace(
            uuid.unwrap(),
            ComponentPtr(static_cast<void*>(data), [](void* ptr) -> void {
                delete static_cast<TyComponent*>(ptr);
            }));

        alertSystemsNewComponentAdded<TyComponent>(uuid);

        DEBUG("added " << type_name<TyComponent>()
                       << " to entity with UUID: " << uuid);
    }

public:
    ComponentManager(const ComponentManager&) = delete;
    ComponentManager& operator=(const ComponentManager&) = delete;
    ComponentManager(ComponentManager&&) = delete;
    ComponentManager& operator=(ComponentManager&&) = delete;

    ComponentManager() {
        bookComponent<Position>(100);
        bookComponent<Velocity>(100);
        bookComponent<Sprite>(100);
        bookComponent<Alliance>(100);
        bookComponent<BoundingSurface>(100);
    }
    ~ComponentManager() = default;

    template <typename TyComponent>
    void add(UUID uuid, const TyComponent& value) {
        insert<TyComponent>(uuid, new TyComponent(value));
    }

    template <typename TyComponent> void add(UUID uuid, TyComponent&& value) {
        insert<TyComponent>(uuid, new TyComponent(value));
    }

    template <typename TyComponent> void add(UUID uuid, TyComponent* value) {
        insert<TyComponent>(uuid, value);
    }

    template <typename TyComponent> void remove(const UUID& uuid) {
        UUIDCompMap& compMap = getCompMap<TyComponent>();

        auto oldValue = compMap.find(uuid.unwrap());

        assert(oldValue != compMap.end());

        compMap.erase(oldValue);
        alertSystemsOldComponentRemoved<TyComponent>(uuid);
    }

    template <typename TyComponent> TyComponent* get(const UUID& uuid) {
        UUIDCompMap& compMap = getCompMap<TyComponent>();

        auto comp = compMap.find(uuid.unwrap());

        if (comp != compMap.end()) {
            void* ptr = comp->second.get();
            return static_cast<TyComponent*>(ptr);
        } else {
            return nullptr;
        }
    }

    template <typename TyComponent> const TyComponent* get(UUID uuid) const {
        const UUIDCompMap& compMap = m_store.at(index<TyComponent>());

        const auto comp = compMap.find(uuid.unwrap());

        if (comp != compMap.end()) {
            const void* ptr = comp->second.get();
            return static_cast<const TyComponent*>(ptr);
        } else {
            return nullptr;
        }
    }

    void destroy(UUID uuid) {
        // TODO: remember that this method will get more complex once
        // entities contain references to other UUIDS (ex: parent/children)
        for (auto& cm : m_store) {
            UUIDCompMap& compMap = cm.second;
            compMap.erase(uuid.unwrap());
        }

        for (auto& p : m_subscribedComponents) {
            System* sys = p.first;
            sys->unfollow(uuid);
        }
    }

    template <typename TyComponent> bool has(UUID uuid) {
        return static_cast<bool>(
            getCompMap<TyComponent>().count(uuid.unwrap()));
    }

    template <typename TyComponent> void subscribe(System* sys) {
        auto idx = index<TyComponent>();

        if (m_subscribedComponents.find(sys) == m_subscribedComponents.end()) {
            m_subscribedComponents[sys] = {};
        }

        m_subscribedSystems[idx].insert(sys);
        m_subscribedComponents[sys].insert(idx);

        DEBUG(sys->name << " system subscribed to: "
                        << type_name<TyComponent>());
    }

    template <typename TyComponent1, typename TyComponent2,
              typename... TyComponents>
    void subscribe(System* s) {
        subscribe<TyComponent1>(s);
        subscribe<TyComponent2, TyComponents...>(s);
    }
};
