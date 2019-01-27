#pragma once

#include <cstdint>
#include <iostream>
#include <set>
#include <algorithm>
#include <vector>
#include <utility>

#include "unstd/types.hpp"

//TODO: UUID graveyard?
// would require keeping 'generation' as part of UUID

class UUID {
public:
    using rep = u32;
    static rep m_nextID;

    UUID(void) : m_ID(m_nextID++) {}

    UUID(const UUID& rhs) : m_ID(rhs.m_ID) {}
    UUID(UUID&& rhs) : m_ID(rhs.m_ID) {}
    inline rep unwrap(void) const { return m_ID; }

	UUID& operator=(const UUID& rhs) {
        m_ID = rhs.m_ID;
        return *this;
	}

    UUID& operator=(UUID&& rhs) {
        m_ID = rhs.m_ID;
        return *this;
    }

    bool operator==(const UUID& rhs) const { return m_ID == rhs.m_ID; }
    bool operator!=(const UUID& rhs) const { return m_ID != rhs.m_ID; }
    friend bool operator<(const UUID& a, const UUID& b) { return a.m_ID < b.m_ID; }
    friend bool operator<=(const UUID& a, const UUID& b) { return a.m_ID <= b.m_ID; }
    friend bool operator>(const UUID& a, const UUID& b) { return a.m_ID > b.m_ID; }
    friend bool operator>=(const UUID& a, const UUID& b) { return a.m_ID >= b.m_ID; }

private:
    rep m_ID;
};

std::ostream& operator<<(std::ostream& os, const UUID& stui);

struct UUIDHasher {
    static u32 hash(UUID::rep x) {
        x = ((x >> 16) ^ x) * 0x45d9f3b;
        x = ((x >> 16) ^ x) * 0x45d9f3b;
        x = (x >> 16) ^ x;
        return x;
    }
};

// class UUIDSet {
//     std::vector<UUID> V;
//     std::less<UUID> cmp;
//
// public:
//     UUIDSet(void) : V(), cmp() {}
//
//     UUIDSet& operator=(const UUIDSet& rhs) = delete;
//     UUIDSet& operator=(UUIDSet&& rhs) = delete;
//     UUIDSet(UUIDSet&&) = delete;
//     UUIDSet(const UUIDSet&) = delete;
//
//     typedef typename std::vector<UUID>::iterator       iterator;
//     typedef typename std::vector<UUID>::const_iterator const_iterator;
//     iterator       begin()       { return V.begin(); }
//     iterator       end()         { return V.end(); }
//     const_iterator begin() const { return V.begin(); }
//     const_iterator end()   const { return V.end(); }
//
//     inline void insert(const UUID& uuid) {
//         iterator i = std::lower_bound(begin(), end(), uuid, cmp);
//         if (i == end() || cmp(uuid, *i))
//             V.insert(i, uuid);
//     }
//
//     inline void erase(UUID uuid) {
//         auto it = std::find(V.begin(), V.end(), uuid);
//
//         if (it != V.end()) {
//             V.erase(it);
//         }
//     }
//
//     inline iterator erase(UUIDSet::const_iterator it) {
//         return V.erase(it);
//     }
//
//     inline bool contains(UUID uuid) const {
// 		return std::find(V.begin(), V.end(), uuid) != V.end();
//     }
//
//     inline size_t size(void) const { return V.size(); }
// };
//
