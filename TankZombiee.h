#pragma once
#include "Zombie.h"

class TankZombiee : public Zombie {
public:
    TankZombiee(float startX, float startY);
    int getDamage() const override { return 2; }
};