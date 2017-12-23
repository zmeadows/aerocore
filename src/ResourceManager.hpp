#pragma once

#include <array>
#include <functional>
#include <iostream>
#include <memory>
#include <set>
#include <utility>
#include <vector>

#include "Base.hpp"
#include "Util.hpp"

class ResourceManager final {
public:
    using Handle = uint_fast64_t;

private:
    std::unique_ptr<char[]> m_data;
    size_t m_capacity;
    std::set<Handle> m_inactive;

    std::function<void(ResourceManager*)> destructor;

    template <typename T>
    inline T* data(void) {
        return reinterpret_cast<T*>(m_data.get());
    }

public:
    ResourceManager() : m_data(nullptr), m_capacity(0), destructor([](ResourceManager*) {}) {}

    inline size_t capacity(void) const { return m_capacity; }

    template <typename T>
    inline void allocate(size_t capacity) {
        DEBUG_ASSERT(!m_data, "Attempted to double-allocate a ResourceManager for type: " << type_name<T>());

        m_data.reset(new char[sizeof(T) * capacity]);
        m_capacity = capacity;

        for (Handle h = 0; h < m_capacity; h++)
            m_inactive.insert(h);

        destructor = [](ResourceManager* rm) {
            for (size_t i = 0; i < rm->capacity(); i++)
                if (!contains(rm->m_inactive, i))
                    rm->get<T>(i)->~T();
        };
    }

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager(ResourceManager&&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    ResourceManager& operator=(ResourceManager&&) = delete;

    ~ResourceManager(void) { destructor(this); }

    template <typename T>
    inline T* get(Handle handle) {
        return &(data<T>()[handle]);
    }

    template <typename T>
    inline const T* get(Handle handle) const {
        return &(data<T>()[handle]);
    }

    inline Handle reserve(void) {
        DEBUG_ASSERT(!m_inactive.empty(), "ResourceManager buffer overflow for type: " << type_name<T>());
        auto hit = m_inactive.begin();
        const Handle newHandle = *hit;
        m_inactive.erase(hit);
        return newHandle;
    }

    template <typename T>
    inline void release(Handle handle) {
        const bool releaseSuccess = m_inactive.insert(handle).second;
        DEBUG_ASSERT(releaseSuccess,
                     "Attempted to release inactive data in ResourceManager for type: " << type_name<T>());
        data<T>()[handle].~T();
    }
};
