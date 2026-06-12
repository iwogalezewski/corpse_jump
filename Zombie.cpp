#include "Zombie.h"
#include <cmath>
#include <iostream>
Zombie::Zombie(float startX, float startY)
    : GameObject(startX, startY), movementSpeed(100.f), hp(3), gravity(1500.f), isGrounded(false) {

    shape.setSize(sf::Vector2f(50.f, 50.f));
    static sf::Texture texture;
    static bool isLoaded = false;

    if (!isLoaded) {
        if (!texture.loadFromFile("zombie.png")) {
            std::cerr << "Blad: Nie mozna wczytac zombie.png!" << std::endl;
        }
        isLoaded = true;
    }

    shape.setTexture(&texture);
    shape.setFillColor(sf::Color::White);
}

void Zombie::update(float deltaTime) {

    velocity.y += gravity * deltaTime;

    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;

    const sf::Texture* currentTexture = shape.getTexture();

    if (currentTexture != nullptr) {
        int imgWidth = currentTexture->getSize().x;
        int imgHeight = currentTexture->getSize().y;

        if (velocity.x > 0.f) {
            shape.setTextureRect(sf::IntRect(0, 0, imgWidth, imgHeight));
        }
        else if (velocity.x < 0.f) {
            shape.setTextureRect(sf::IntRect(imgWidth, 0, -imgWidth, imgHeight));
        }
    }

    shape.setPosition(position);
}

void Zombie::moveTowards(float targetX) {

    float centerX = position.x + shape.getSize().x / 2.0f;

    if (std::abs(targetX - centerX) > 5.f) {
        if (centerX < targetX) {
            velocity.x = movementSpeed;
        }
        else {
            velocity.x = -movementSpeed; 
        }
    }
    else {
        velocity.x = 0.f; 
    }
}

void Zombie::takeDamage(int damage) {
    hp -= damage;
    if (hp <= 0) {
        markForDeletion(); 
    }
}

void Zombie::setGrounded(bool state) {
    isGrounded = state;
}

void Zombie::resolveCollision(const sf::FloatRect& platformBounds) {
    sf::FloatRect zombieBounds = getBounds();
    if (velocity.y > 0.f && (zombieBounds.top < platformBounds.top)) {
        position.y = platformBounds.top - zombieBounds.height;
        velocity.y = 0.f;
        isGrounded = true;
        shape.setPosition(position);
    }
}