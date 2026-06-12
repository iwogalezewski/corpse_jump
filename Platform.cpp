#include "Platform.h"
#include <iostream>
Platform::Platform(float startX, float startY, float width, float height)
    : GameObject(startX, startY) {
    shape.setSize(sf::Vector2f(width, height));

    static sf::Texture texture;
    static bool isLoaded = false;

    if (!isLoaded) {
        if (!texture.loadFromFile("platform.png")) {
            std::cerr << "Blad: Nie mozna wczytac platform.png!" << std::endl;
        }
    }

    shape.setTexture(&texture);

    shape.setFillColor(sf::Color::White);
}

void Platform::update(float deltaTime) {

}