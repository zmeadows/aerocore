#include "System/DamageSystem.hpp"

#include "Component/Common.hpp"
#include "Globals.hpp"

void run(DamageSystem& self) {
    auto CM = get_manager();

    for (const UUID uuid : self.base.followed) {
        append(self.followed_copy, uuid);
    }

    for (const UUID& uuid : self.base.followed) {
        auto& damage = CM->get<DamageEvent>(uuid);
        auto& health = CM->get<Health>(uuid);

        health.modify_by(-1.f * damage.amount);

        if (health.current <= 0.f)
            CM->book<DestructTag>(uuid);
    }

    for (const UUID uuid : self.followed_copy) {
        CM->remove<DamageEvent>(uuid);
    }

    clear(self.followed_copy);
}
