#pragma once
#include "GameObject.h"

class Bullet : public GameObject {
private:
    sf::Vector2f direction;
    float speed;
    int damage;

public:
    Bullet(float startX, float startY, sf::Vector2f dir, int dmg = 1);
    void update(float deltaTime) override;
    int getDamage() const {return damage;}
};