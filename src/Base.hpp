#pragma once

#include <cmath>
#include <iostream>

#ifdef DODEBUG
#define DEBUG(x)                                                                                             \
    (std::cout << std::boolalpha << "[" << __FILE__ << "::" << __LINE__ << "]\t" << x << std::endl)
#else
#define DEBUG(x)
#endif

enum class EntityType { Player, Bullet, Enemy };

enum class Alliance { Friend, Foe, Neutral };

struct Position {
    float x = 0;
    float y = 0;
};

struct Velocity {
    float x = 0.0;
    float y = 0.0;
};

struct Acceleration {
    float x = 0.0;
    float y = 0.0;
};

enum class OffscreenBehavior { DiesInstantly, Wraps };
