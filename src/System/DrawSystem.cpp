#include "System/DrawSystem.hpp"

#include "Component/Common.hpp"
#include "Engine/ComponentManager.hpp"
#include "GraphicsContext.hpp"
#include "unstd/types.hpp"

void DrawSystem::run(ComponentManager* CM, f32 dt) {
    // draw background
    GPU_ClearRGB(GC->renderer, 20, 20, 20);
    GPU_CircleFilled(GC->renderer,
                     GC->convert_to_screen_distance(100.f),
                     GC->convert_to_screen_distance(100.f),
                     100,
                     { 40,40,40,255});

    for (const UUID uuid : this->followed) {
        const Entity& entity = CM->get<Entity>(uuid);
        const Sprite& sprite = CM->get<Sprite>(uuid);

        bool do_flash = false;
        // TODO: create get_ptr style function to avoid double lookups like this
        if (CM->has<Invincibility>(uuid)) {
            do_flash = CM->get<Invincibility>(uuid).is_flashing;
        }

        ScreenCoordinates sc = GC->to_screen_coords(entity.pos);
        const v2 offset = sprite.offset.rotated(-entity.angle);
        if (do_flash) GPU_SetRGB(sprite.image, 255, 0, 0);
        GPU_BlitRotate(sprite.image, NULL, GC->renderer,
                       sc.x - offset.x, sc.y - offset.y,
                       - entity.angle * 180.f/PI);
        if (do_flash) GPU_UnsetColor(sprite.image);
    }
}

