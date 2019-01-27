#include "System/InvincibilitySystem.hpp"

#include "Component/Common.hpp"
#include "Globals.hpp"

void run(InvincibilitySystem& self, f32 dt) {
    auto CM = get_manager();

    for (const UUID uuid : self.base.followed) {
        auto& sprite = CM->get<Sprite>(uuid);
        auto& flash = CM->get<Invincibility>(uuid);

        flash.timer -= dt;

        if (flash.timer <= 0.f) {
            if (flash.is_flashing) flash.num_flashes--;
            flash.is_flashing = !flash.is_flashing;
            flash.timer += 0.1;
        }

		if (flash.num_flashes == 0)
            append(self.finished, uuid);
    }

	for (const UUID uuid : self.finished) {
        CM->remove<Invincibility>(uuid);
	}

    clear(self.finished);
}
