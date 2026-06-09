#include "GameObject.h"

GameObject::GameObject(float startX, float startY)
    : position(startX, startY), velocity(0.f, 0.f) {
    // Domyœlne ustawienia kszta³tu 
    shape.setPosition(position);
    shape.setSize(sf::Vector2f(50.f, 50.f));
    shape.setFillColor(sf::Color::White);
}

void GameObject::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::FloatRect GameObject::getBounds() const {
    return shape.getGlobalBounds();
}