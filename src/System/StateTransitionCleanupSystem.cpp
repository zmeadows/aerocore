#include "System/StateTransitionCleanupSystem.hpp"

#include "Globals.hpp"

void run(StateTransitionCleanupSystem& self) {
    auto CM = get_manager();

    for (const UUID& uuid : self.base.followed) {
        append(&self.followed_copy, uuid);
    }

    for (const UUID uuid : self.followed_copy) {
        CM->remove<StateTransition>(uuid);
    }

    clear(&self.followed_copy);
}
