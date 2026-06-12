#pragma once
#include "GameObject.h"

class Player : public GameObject {
private:
    float movementSpeed;
    float jumpForce;
    float gravity;
    bool isGrounded;
    sf::Texture playerTexture;
    sf::IntRect textureRect;
    bool isFacingRight = true;
    int specialAmmo = 0;

    sf::RectangleShape gun;

public:
    Player(float startX, float startY);

    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override; 
    void setGrounded(bool state);
    void resolveCollision(const sf::FloatRect& platformBounds);
    void addSpecialAmmo(int amount) { specialAmmo += amount; }
    bool hasSpecialAmmo() const { return specialAmmo > 0; }
    void useSpecialAmmo() {
        if (specialAmmo > 0)
            specialAmmo--;
    }

    void aim(sf::Vector2f mousePos); 
    sf::Vector2f getCenter() const; 
};