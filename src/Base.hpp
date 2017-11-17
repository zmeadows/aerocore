#pragma once

#ifdef DODEBUG
#define DEBUG(x)                                                        \
    (std::cout << std::boolalpha << "[" << __FILE__ << "::" << __LINE__ \
               << "]\t" << x << std::endl)
#else
#define DEBUG(x)
#endif

enum class EntityType {
    Player,
    Bullet,
    Enemy
};

struct Position {
    float x = 0;
    float y = 0;
};

struct PixelCoordinates {
    int x;
    int y;
};

// std::pair<int,int>
// toPixelCoordinates(const Position& pos, const size_t windowWidth)
// {
//     int wOtwo = windowWidth / 2;
//     int dx = 
// 
//     return std::make_pair(wOtwo + pos.)
// }

struct Velocity {
    float x;
    float y;
};

struct FiringRate {
};