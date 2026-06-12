#pragma once
#include <SFML/Graphics.hpp>

class GameObject {
protected:
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::RectangleShape shape;

    bool dead = false; 

public:
    GameObject(float startX, float startY);
    virtual ~GameObject() = default;

    virtual void update(float deltaTime) = 0;
    virtual void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;

    bool isMarkedForDeletion() const { return dead; }
    virtual void markForDeletion() { dead = true; }
};