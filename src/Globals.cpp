#include "Globals.hpp"

namespace __asteroids__globals__ {

GraphicsContext    g_GC = GraphicsContext();
AudioContext       g_AC = AudioContext();
ComponentManager   g_CM = ComponentManager();
QuadTree           g_QT = QuadTree(3);
SpriteCache        g_SC = SpriteCache();

}

using namespace __asteroids__globals__;

ComponentManager* get_manager(void)         { return &g_CM; }
GraphicsContext* get_graphics_context(void) { return &g_GC; }
QuadTree* get_quad_tree(void)               { return &g_QT; }
AudioContext* get_audio_context(void)       { return &g_AC; }
SpriteCache* get_sprite_cache(void)         { return &g_SC; }

//TODO: just play sound, don't create this additional SoundEvent component layer?
void play_sound(SoundEffect effect) {
    auto CM = get_manager();
    UUID uuid;
    CM->book<SoundEvent>(uuid, effect);
}
