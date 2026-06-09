#pragma once
#include "GameObject.h"

class Zombie : public GameObject {
private:
    float movementSpeed;
    int hp;
    float gravity;
    bool isGrounded;

public:
    Zombie(float startX, float startY);

    void update(float deltaTime) override;

    // Obs³uga fizyki (podobnie jak u gracza)
    void setGrounded(bool state);
    void resolveCollision(const sf::FloatRect& platformBounds);

    // Mechaniki zombie
    void takeDamage(int damage);
    void moveTowards(float targetX);
};