#pragma once
#include "GameObject.h"

class Player : public GameObject {
private:
    float movementSpeed;
    float jumpForce;
    float gravity;
    bool isGrounded;

    sf::RectangleShape gun;

public:
    Player(float startX, float startY);

    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override; 
    void setGrounded(bool state);
    void resolveCollision(const sf::FloatRect& platformBounds);

    void aim(sf::Vector2f mousePos); 
    sf::Vector2f getCenter() const; 
};