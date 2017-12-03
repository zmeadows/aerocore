#pragma once

#include <cstdint>
#include <iostream>
#include <unordered_set>

class UUID {
public:
    using rep = uint_fast64_t;

    UUID(void) : m_ID(m_nextID) { m_nextID++; }

    UUID(const UUID& rhs) : m_ID(rhs.m_ID) {}
    UUID(UUID&& rhs) : m_ID(rhs.m_ID) {}
    inline rep unwrap(void) const { return m_ID; }

    UUID& operator=(const UUID& rhs) = delete;
    UUID& operator=(UUID&& rhs) = delete;

    inline bool operator==(const UUID& rhs) { return m_ID == rhs.m_ID; }
    inline bool operator!=(const UUID& rhs) { return m_ID != rhs.m_ID; }
    inline friend bool operator<(const UUID& a, const UUID& b) { return a.m_ID < b.m_ID; }

    static const UUID playerUUID;

private:
    friend class UUIDSet;
    UUID(UUID::rep value) : m_ID(value) {}
    static rep m_nextID;
    const rep m_ID;
};

std::ostream& operator<<(std::ostream& os, const UUID& stui);

class UUIDSet {
    std::unordered_set<UUID::rep> m_set;

public:
    class iterator {
        using myiter_t = std::unordered_set<UUID::rep>::const_iterator;
        myiter_t m_iter;

    public:
        iterator(myiter_t i) : m_iter(i) {}

        iterator operator++(void) {
            m_iter++;
            return *this;
        }
        bool operator!=(const iterator& rhs) { return m_iter != rhs.m_iter; }
        const UUID operator*(void) { return UUID(*m_iter); }
        friend iterator next(iterator rhs) { return ++rhs; }
    };

    UUIDSet(void) {}

    UUIDSet& operator=(const UUIDSet& rhs) = delete;
    UUIDSet& operator=(UUIDSet&& rhs) = delete;
    UUIDSet(UUIDSet&&) = delete;
    UUIDSet(const UUIDSet&) = delete;

    inline void insert(const UUID& uuid) { m_set.insert(uuid.unwrap()); }
    inline void erase(const UUID& uuid) { m_set.erase(uuid.unwrap()); }
    inline bool contains(const UUID& uuid) const { return static_cast<bool>(m_set.count(uuid.unwrap())); }

    iterator begin(void) const { return iterator(m_set.begin()); }
    iterator end(void) const { return iterator(m_set.end()); }
};
