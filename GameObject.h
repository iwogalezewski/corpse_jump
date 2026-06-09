#pragma once
#include <SFML/Graphics.hpp>

class GameObject {
protected:
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::RectangleShape shape;

    bool dead = false; // Nowa flaga informuj¹ca, czy obiekt do usuniêcia

public:
    GameObject(float startX, float startY);
    virtual ~GameObject() = default;

    virtual void update(float deltaTime) = 0;
    virtual void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;

    // Metody do zarz¹dzania ¿yciem obiektu
    bool isMarkedForDeletion() const { return dead; }
    virtual void markForDeletion() { dead = true; }
};