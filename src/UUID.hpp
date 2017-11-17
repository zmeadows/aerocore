#pragma once

#include <cstdint>
#include <iostream>

class UUID {
public:
    using rep = uint_fast64_t;

    UUID(void) : m_ID(m_nextID) { m_nextID++; }
    UUID(const UUID& rhs) : m_ID(rhs.m_ID) {}
    UUID(UUID&& rhs) : m_ID(rhs.m_ID) {}
    inline rep unwrap(void) const { return m_ID; }

    UUID &operator=(const UUID &rhs) = delete;
    UUID &operator=(UUID &&rhs) = delete;

    inline bool operator==(const UUID &rhs) { return m_ID == rhs.m_ID; }
    inline bool operator!=(const UUID &rhs) { return m_ID != rhs.m_ID; }
    inline friend bool operator<(const UUID &a, const UUID &b) { return a.m_ID < b.m_ID; }

    inline friend std::ostream &operator<<(std::ostream &os, const UUID &stui);

    static const UUID playerUUID;

private:
    static rep m_nextID;
    const rep m_ID;
};

UUID::rep UUID::m_nextID = 1;
const UUID UUID::playerUUID;

std::ostream &operator<<(std::ostream &os, const UUID &stui) {
  std::ios_base::fmtflags oldFlags(os.flags());
  os << std::hex << stui.m_ID;
  os.flags(oldFlags);
  return os;
}
