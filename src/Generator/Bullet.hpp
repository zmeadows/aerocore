#pragma once

struct v2;
class UUID;

UUID generateBullet(const v2 new_bullet_position, const v2 new_bullet_velocity, float new_bullet_angle);
