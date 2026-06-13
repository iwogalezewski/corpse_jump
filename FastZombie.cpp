#include "FastZombie.h"
#include <iostream>
#include <cmath> 

FastZombie::FastZombie(float startX, float startY) : Zombie(startX, startY) {
    movementSpeed = 250.f;
    hp = 1;
    jumpForce = 850.f;  
    jumpCooldown = 0.f; 

    shape.setSize(sf::Vector2f(30.f, 30.f));

    static sf::Texture texture;
    static bool isLoaded = false;

    if (!isLoaded) {
        if (!texture.loadFromFile("fast_zombie.png")) {
            std::cerr << "Blad: Nie mozna wczytac fast_zombie.png!" << std::endl;
        }
        isLoaded = true;
    }

    shape.setTexture(&texture);
    shape.setFillColor(sf::Color::White);
}

void FastZombie::update(float deltaTime) {
    Zombie::update(deltaTime);

    if (jumpCooldown > 0.f) {
        jumpCooldown -= deltaTime;
    }

    if (isGrounded && jumpCooldown <= 0.f && targetPos.y < position.y - 40.f) {
        if (std::abs(targetPos.x - position.x) < 300.f) {
            velocity.y = -jumpForce;
            isGrounded = false;
            jumpCooldown = 1.0f;
        }
    }
}