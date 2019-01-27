
#include "AudioContext.hpp"
#include "ComponentManager.hpp"
#include "GraphicsContext.hpp"
#include "QuadTree.hpp"
#include "SpriteCache.hpp"

#define ACTIVATE_SYSTEM(sys) get_manager()->subscribe(decltype(sys)::subscriptions, &sys.base)

// #define DEACTIVATE_SYSTEM(sys) //TODO

namespace __asteroids__globals__ {

extern GraphicsContext g_GC;
extern AudioContext g_AC;
extern ComponentManager g_CM;
extern QuadTree g_QT;
extern SpriteCache g_SC;

}

ComponentManager* get_manager(void);
GraphicsContext* get_graphics_context(void);
QuadTree* get_quad_tree(void);
AudioContext* get_audio_context(void);
SpriteCache* get_sprite_cache(void);

