#include "Globals.hpp"

namespace __asteroids__globals__ {

ComponentManager g_CM = ComponentManager();
GraphicsContext g_GC = GraphicsContext();
QuadTree g_QT = QuadTree(3);
AudioContext g_AC = AudioContext();

}

using namespace __asteroids__globals__;

ComponentManager* get_manager(void) { return &g_CM; }
GraphicsContext* get_graphics_context(void) { return &g_GC; }
QuadTree* get_quad_tree(void) { return &g_QT; }

void play_sound(SoundEffect effect) { g_AC.play(effect); }
