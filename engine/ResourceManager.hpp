#pragma once

#include "Util.hpp"

#include <functional>

//TODO: create ResourceManagerBase class and template ResourceManager
// This way I can remove the need for the release_func

class ResourceManager final {
public:
    using Handle = size_t;

private:
    std::unique_ptr<char[]> m_data;
    size_t m_capacity;
    std::set<Handle> m_inactive;
    std::function<void(Handle)> release_func;

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
    ResourceManager() : m_data(nullptr), m_capacity(0) {}

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager(ResourceManager&&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    ResourceManager& operator=(ResourceManager&&) = delete;

    //TODO: add destructor

    // TODO: make ResourceManager have resizable capacity

    template <typename T>
    void allocate(size_t capacity) {
        assert(m_data.get() == nullptr && "Attempted to double-allocate a ResourceManager for type: ");

        m_data.reset(new char[sizeof(T) * capacity]);
        m_capacity = capacity;

        for (Handle h = 0; h < m_capacity; h++)
            m_inactive.insert(h);

        release_func = [this](Handle h) {
			T& x = this->get<T>(h);
            x.~T();
        };
    }

    template <typename T>
    void deallocate(void) {
        assert(m_data.get() && "Attempted to deallocate an unallocated ResourceManager for type: ");

        for (size_t i = 0; i < m_capacity; i++)
            if (!contains(m_inactive, i))
                get<T>(i).~T();
        m_data.release();
    }

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

    inline void release(Handle handle) {
        release_func(handle);
        // const bool releaseSuccess = m_inactive.insert(handle).second;
        static_cast<void>(m_inactive.insert(handle).second);
        // assert(releaseSuccess && "Attempted to release inactive data in ResourceManager for type: ");
    }
};
