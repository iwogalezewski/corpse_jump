#include "Platform.h"

Platform::Platform(float startX, float startY, float width, float height)
    : GameObject(startX, startY) {
    shape.setSize(sf::Vector2f(width, height));
    shape.setFillColor(sf::Color(139, 69, 19)); 
}

void Platform::update(float deltaTime) {

}