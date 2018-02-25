
#include "AudioContext.hpp"
#include "ComponentManager.hpp"
#include "GraphicsContext.hpp"
#include "QuadTree.hpp"

namespace __asteroids__globals__ {

extern GraphicsContext g_GC;
extern AudioContext g_AC;
extern ComponentManager g_CM;
extern QuadTree g_QT;

}

ComponentManager* get_manager(void);
GraphicsContext* get_graphics_context(void);
QuadTree* get_quad_tree(void);
void play_sound(SoundEffect effect);
