#include "Entity.hpp"

void destroy_entity(ComponentManager* CM, const UUID& uuid) {
    if (CM->has<CollisionData>(uuid)) {
        QuadNode* node = CM->get<CollisionData>(uuid).node;
        if (node) node->release_entity(uuid);
    }
    CM->destroy(uuid);
}
