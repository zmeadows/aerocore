#include "UUID.hpp"

UUID::rep UUID::m_nextID = 0;
const UUID UUID::playerUUID;

std::ostream& operator<<(std::ostream& os, const UUID& stui)
{
    std::ios_base::fmtflags oldFlags(os.flags());
    os << std::hex << stui.unwrap();
    os.flags(oldFlags);
    return os;
}
