#pragma once
#include "GameObject.h"

class Bullet : public GameObject {
private:
    sf::Vector2f direction;
    float speed;

public:
    Bullet(float startX, float startY, sf::Vector2f dir);
    void update(float deltaTime) override;
};