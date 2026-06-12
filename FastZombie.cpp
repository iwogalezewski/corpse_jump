#include "FastZombie.h"
#include <iostream>
FastZombie::FastZombie(float startX, float startY) : Zombie(startX, startY) {
    movementSpeed = 250.f; 
    hp = 1;

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