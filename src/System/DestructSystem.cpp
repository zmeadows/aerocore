#include "System/DestructSystem.hpp"

#include "Globals.hpp"

void run(DestructSystem& self) {
    ComponentManager* CM = get_manager();

    for (const UUID& uuid : self.base.followed)
        append(self.followed_copy, uuid);

    for (const UUID& uuid : self.followed_copy) {
        CM->destroy(uuid);
    }

    clear(self.followed_copy);
}
