#pragma once

struct v2;
class UUID;

UUID generate_bullet_player(const v2 new_bullet_position, const v2 new_bullet_velocity, float new_bullet_angle);
UUID generate_bullet_enemy(const v2 new_bullet_position, const v2 new_bullet_velocity, float new_bullet_angle);
