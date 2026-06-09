#pragma once
#include "GameObject.h"

class Player : public GameObject {
private:
    float movementSpeed;
    float jumpForce;
    float gravity;
    bool isGrounded;

    sf::RectangleShape gun; // Lufa pistoletu

public:
    Player(float startX, float startY);

    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override; // Nadpisujemy draw, ¿eby narysowaæ te¿ broñ

    void setGrounded(bool state);
    void resolveCollision(const sf::FloatRect& platformBounds);

    void aim(sf::Vector2f mousePos); // Celowanie
    sf::Vector2f getCenter() const;  // Pobranie œrodka gracza
};