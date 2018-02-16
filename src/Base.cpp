#include "Base.hpp"

const UUID playerUUID(void) {
    static UUID id;
    return id;
}
