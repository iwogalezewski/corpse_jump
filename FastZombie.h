#pragma once
#include "Zombie.h"

class FastZombie : public Zombie {
private:
    float jumpForce;
    float jumpCooldown;

public:
    FastZombie(float startX, float startY);
    void update(float deltaTime) override;
};