#include "TankZombiee.h"

TankZombiee::TankZombiee(float startX, float startY) : Zombie(startX, startY) {
    movementSpeed = 35.f;
    hp = 15;
    shape.setSize(sf::Vector2f(90.f, 90.f));
    shape.setFillColor(sf::Color(105, 105, 105));
}