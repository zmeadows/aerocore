#pragma once

#include <cassert>
#include <array>
#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "UUID.hpp"

namespace {

#define DODEBUG

#ifdef DODEBUG
#define DEBUG(x)                                                                                             \
    (std::cout << std::boolalpha << "[" << __FILE__ << "::" << __LINE__ << "] :: " << x << std::endl)
#else
#define DEBUG(x)
#endif

template <typename Container, typename T>
inline bool contains(const Container& cont, const T& data) {
    return cont.find(data) != cont.end();
}

template <class T>
constexpr
std::string_view
type_name()
{
    using namespace std;
#ifdef __clang__
    string_view p = __PRETTY_FUNCTION__;
    // return string_view(p.data() + 34, p.size() - 34 - 1);
    return string_view(p.data() + 57, p.size() - 57 - 1);
#elif defined(__GNUC__)
    string_view p = __PRETTY_FUNCTION__;
#  if __cplusplus < 201402
    return string_view(p.data() + 36, p.size() - 36 - 1);
#  else
    return string_view(p.data() + 49, p.find(';', 49) - 49);
#  endif
#elif defined(_MSC_VER)
    string_view p = __FUNCSIG__;
    return string_view(p.data() + 84, p.size() - 84 - 7);
#endif
}

} // namespace

namespace aerocore {

class ResourceManager final {
public:
    using Handle = size_t;

private:
    std::unique_ptr<char[]> m_data;
    size_t m_capacity;
    std::set<Handle> m_inactive;

    std::function<void(ResourceManager*)> destructor;

    template <typename T>
    inline T* data(void) {
        return reinterpret_cast<T*>(m_data.get());
    }

    inline Handle reserve(void) {
        auto hit = m_inactive.begin();
        const Handle newHandle = *hit;
        m_inactive.erase(hit);
        return newHandle;
    }

public:
    ResourceManager() : m_data(nullptr), m_capacity(0), destructor([](ResourceManager*) {}) {}

    inline size_t capacity(void) const { return m_capacity; }

    // TODO: make ResourceManager have resizable capacity

    template <typename T>
    inline void allocate(size_t capacity) {
        assert(m_data.get() == nullptr && "Attempted to double-allocate a ResourceManager for type: ");

        m_data.reset(new char[sizeof(T) * capacity]);
        m_capacity = capacity;

        for (Handle h = 0; h < m_capacity; h++)
            m_inactive.insert(h);

        destructor = [](ResourceManager* rm) {
            for (size_t i = 0; i < rm->capacity(); i++)
                if (!contains(rm->m_inactive, i))
                    rm->get<T>(i).~T();
            rm->m_data.release();
        };
    }

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager(ResourceManager&&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    ResourceManager& operator=(ResourceManager&&) = delete;

    ~ResourceManager(void) { destructor(this); }

    template <typename T, class... Args>
    inline Handle book(Args&&... args) {
        const Handle newHandle = this->reserve();
        new (&(data<T>()[newHandle])) T(args...);
        return newHandle;
    }

    template <typename T>
    inline T& get(Handle handle) {
        return data<T>()[handle];
    }

    template <typename T>
    inline const T& get(Handle handle) const {
        return data<T>()[handle];
    }

    template <typename T>
    inline void release(Handle handle) {
        data<T>()[handle].~T();
        const bool releaseSuccess = m_inactive.insert(handle).second;
        assert(releaseSuccess && "Attempted to release inactive data in ResourceManager for type: ");
    }
};


class ComponentManager;

class System {
protected:
    UUIDSet m_followed;

    ComponentManager* const CM;

    System(const std::string& name_, ComponentManager* const CM_)
        : CM(CM_), name(name_) {}

public:
    const std::string name;

    virtual void run(float dt) = 0;

    inline bool isFollowing(const UUID& uuid) const {
        return static_cast<bool>(m_followed.contains(uuid));
    }

    inline void follow(const UUID& uuid) {
        m_followed.insert(uuid);
        DEBUG(name << " system followed UUID: " << uuid);
    }

    inline void unfollow(const UUID& uuid) {
        m_followed.erase(uuid);
        DEBUG(name << " system unfollowed UUID: " << uuid);
    }

    virtual ~System(void) {}
};


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

    ComponentIndex newIndex(void) {
        ComponentIndex idx = m_nextIndex;
        m_nextIndex++;
        m_allComponentIndices.insert(idx);
        return idx;
    }

    ////////////////////////////
    // Component Data Storage //
    ////////////////////////////

    // TODO: switch to an array of handles, with size == max number of entities
    // will be a little tough and bug-prone to implement, since we would have to re-use UUIDs
    // and implement a UUID pool
    using UUIDCompMap = std::unordered_map<UUID::rep, ResourceManager::Handle>;
    std::vector<UUIDCompMap> m_store;
    std::vector<std::unique_ptr<ResourceManager>> m_pools;

    /////////////////////////////////////////
    // System <--> Component Subscriptions //
    /////////////////////////////////////////

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

    ComponentManager() : m_nextIndex(0) {
        m_pools.reserve(1000);
        m_store.reserve(1000);
    }

    ~ComponentManager() = default;

    template <typename TyComponent>
    void registerComponent(const size_t allocSize);

    template <typename TyComponent, class... Args>
    TyComponent& book(const UUID& uuid, Args&&... args);

    template <typename TyComponent>
    void remove(const UUID& uuid);

    template <typename TyComponent>
    TyComponent& get(const UUID& uuid);

    template <typename TyComponent>
    const TyComponent& get(UUID uuid) const;

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

    assert(m_store.size() == newCompID &&
           "Expected/Actual mismatch between number of registered component stores when registering ");
    assert(m_pools.size() == newCompID &&
           "Expected/Actual mismatch between number of registered component pools when registering ");

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

template <typename TyComponent, class... Args>
TyComponent& ComponentManager::book(const UUID& uuid, Args&&... args) {
    const ComponentIndex compID = index<TyComponent>();

    UUIDCompMap& compMap = m_store[compID];

    DEBUG(uuid << " booked component: " << type_name<TyComponent>());

    assert(!contains(compMap, uuid.unwrap()) &&
           "Attempted to book component for entity that already posseses it");

    const ResourceManager::Handle handle = m_pools[compID]->book<TyComponent>(args...);
    compMap[uuid.unwrap()] = handle;

    alertSystemsNewComponentAdded<TyComponent>(uuid);
    return m_pools[compID]->get<TyComponent>(handle);
}

template <typename TyComponent>
void ComponentManager::remove(const UUID& uuid) {
    const ComponentIndex compID = index<TyComponent>();

    UUIDCompMap& compMap = m_store[compID];
    auto oldValue = compMap.find(uuid.unwrap());
    assert(oldValue != compMap.end() && "Attempted to remove non-existent component from entity");
    const ResourceManager::Handle oldHandle = oldValue->second;

    compMap.erase(oldValue);

    m_pools[compID]->release<TyComponent>(oldHandle);

    alertSystemsOldComponentRemoved<TyComponent>(uuid);
}

template <typename TyComponent>
TyComponent& ComponentManager::get(const UUID& uuid) {
    assert(componentIsRegistered<TyComponent>() &&
           "Attempted to 'get' component data for un-registered component of type: ");

    const ComponentIndex compID = index<TyComponent>();

    UUIDCompMap& compMap = m_store[compID];

    auto hit = compMap.find(uuid.unwrap());
    assert(hit != compMap.end());
    return m_pools[compID]->get<TyComponent>(hit->second);
}

template <typename TyComponent>
const TyComponent& ComponentManager::get(UUID uuid) const {
    const ComponentIndex compID = index<TyComponent>();

    assert(compID < m_store.size() && compID < m_pools.size() &&
           "Attempted to 'get' component data for un-registered component of type: ");

    const UUIDCompMap& compMap = m_store.at(compID);

    const auto hit = compMap.find(uuid.unwrap());
    assert(hit != compMap.end());
    return m_pools[compID]->get<TyComponent>(hit->second);
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

} // namespace aerocore
