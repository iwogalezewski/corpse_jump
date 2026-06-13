#pragma once
#include "GameObject.h"

class Zombie : public GameObject {
protected:
    float movementSpeed;
    int hp;
    float gravity;
    bool isGrounded;
    sf::Vector2f targetPos; 

public:
    Zombie(float startX, float startY);

    void update(float deltaTime) override;
    void setGrounded(bool state);
    void resolveCollision(const sf::FloatRect& platformBounds);
    void takeDamage(int damage);

    virtual void moveTowards(sf::Vector2f playerPos);

    virtual int getDamage() const { return 1; }
};