#include "Globals.hpp"

namespace __asteroids__globals__ {

GraphicsContext g_GC = GraphicsContext();
AudioContext g_AC = AudioContext();
ComponentManager g_CM = ComponentManager();
QuadTree g_QT = QuadTree(4);

}

using namespace __asteroids__globals__;

ComponentManager* get_manager(void)         { return &g_CM; }
GraphicsContext* get_graphics_context(void) { return &g_GC; }
QuadTree* get_quad_tree(void)               { return &g_QT; }
AudioContext* get_audio_context(void)       { return &g_AC; }

void play_sound(SoundEffect effect) {
    static ComponentManager* CM = get_manager();
    UUID uuid;
    CM->book<SoundEvent>(uuid, effect);
}
