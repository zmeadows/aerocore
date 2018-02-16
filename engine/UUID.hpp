#pragma once

#include <cstdint>
#include <iostream>
#include <set>

class UUID {
public:
    using rep = uint_fast64_t;
    static rep m_nextID;

    UUID(void) : m_ID(m_nextID++) {}

    UUID(const UUID& rhs) : m_ID(rhs.m_ID) {}
    UUID(UUID&& rhs) : m_ID(rhs.m_ID) {}
    rep unwrap(void) const { return m_ID; }

    UUID& operator=(const UUID& rhs) = delete;
    UUID& operator=(UUID&& rhs) = delete;

    bool operator==(const UUID& rhs) const { return m_ID == rhs.m_ID; }
    bool operator!=(const UUID& rhs) const { return m_ID != rhs.m_ID; }
    friend bool operator<(const UUID& a, const UUID& b) { return a.m_ID < b.m_ID; }

private:
    friend class UUIDSet;
    UUID(UUID::rep value) : m_ID(value) {}
    const rep m_ID;
};

std::ostream& operator<<(std::ostream& os, const UUID& stui);

class UUIDSet {
    std::set<UUID::rep> m_set;

public:
    class iterator {
        using myiter_t = std::set<UUID::rep>::const_iterator;
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

    void insert(const UUID& uuid) { m_set.insert(uuid.unwrap()); }
    void erase(const UUID& uuid) { m_set.erase(uuid.unwrap()); }
    bool contains(const UUID& uuid) const { return static_cast<bool>(m_set.count(uuid.unwrap())); }
    size_t size(void) const { return m_set.size(); }

    iterator begin(void) const { return iterator(m_set.begin()); }
    iterator end(void) const { return iterator(m_set.end()); }
};
