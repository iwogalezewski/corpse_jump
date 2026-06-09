#include "Zombie.h"
#include <cmath> // Do std::abs

Zombie::Zombie(float startX, float startY)
    : GameObject(startX, startY), movementSpeed(100.f), hp(3), gravity(1500.f), isGrounded(false) {
    // Zombiak bêdzie fioletowy i trochê wiêkszy, by ³atwiej by³o w niego trafiæ
    shape.setSize(sf::Vector2f(50.f, 50.f));
    shape.setFillColor(sf::Color::Magenta);
}

void Zombie::update(float deltaTime) {
    // Aplikowanie grawitacji
    velocity.y += gravity * deltaTime;

    // Aktualizacja pozycji
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;

    shape.setPosition(position);
}

void Zombie::moveTowards(float targetX) {
    // Obliczamy œrodek zombiaka
    float centerX = position.x + shape.getSize().x / 2.0f;

    // Zombiak ma ma³¹ "martw¹ strefê" (5 pikseli), ¿eby nie drga³ w miejscu, gdy dojdzie do gracza
    if (std::abs(targetX - centerX) > 5.f) {
        if (centerX < targetX) {
            velocity.x = movementSpeed; // IdŸ w prawo
        }
        else {
            velocity.x = -movementSpeed; // IdŸ w lewo
        }
    }
    else {
        velocity.x = 0.f; // Stój, jesteœ przy celu
    }
}

void Zombie::takeDamage(int damage) {
    hp -= damage;
    if (hp <= 0) {
        markForDeletion(); // Jeœli HP spadnie do 0, oznacz do usuniêcia z pamiêci
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