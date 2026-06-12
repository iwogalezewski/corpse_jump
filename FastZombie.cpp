#include "FastZombie.h"

FastZombie::FastZombie(float startX, float startY) : Zombie(startX, startY) {
    movementSpeed = 250.f; 
    hp = 1;               

    shape.setSize(sf::Vector2f(30.f, 30.f)); 
    shape.setFillColor(sf::Color::Cyan);     
}