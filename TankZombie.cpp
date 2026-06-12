#include "TankZombiee.h"
#include <iostream>
TankZombiee::TankZombiee(float startX, float startY) : Zombie(startX, startY) {
    movementSpeed = 35.f;
    hp = 15;
    shape.setSize(sf::Vector2f(90.f, 90.f));

    static sf::Texture texture;
    static bool isLoaded = false;

    if (!isLoaded) {
        if (!texture.loadFromFile("tank_zombie.png")) {
            std::cerr << "Blad: Nie mozna wczytac tank_zombie.png!" << std::endl;
        }
        isLoaded = true;
    }

    shape.setTexture(&texture);
    shape.setFillColor(sf::Color::White);
}