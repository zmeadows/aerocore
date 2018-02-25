#include "Globals.hpp"

namespace __asteroids__globals__ {

GraphicsContext g_GC = GraphicsContext();
AudioContext g_AC = AudioContext();
ComponentManager g_CM = ComponentManager();
QuadTree g_QT = QuadTree(3);

}

using namespace __asteroids__globals__;

ComponentManager* get_manager(void) { return &g_CM; }
GraphicsContext* get_graphics_context(void) { return &g_GC; }
QuadTree* get_quad_tree(void) { return &g_QT; }

void play_sound(SoundEffect effect) { g_AC.play(effect); }
