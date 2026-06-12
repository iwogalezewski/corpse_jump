#include "Player.h"
#include <cmath> 

Player::Player(float startX, float startY)
    : GameObject(startX, startY), movementSpeed(300.f), jumpForce(950.f), gravity(1500.f), isGrounded(false) {
    shape.setFillColor(sf::Color::Green);

    gun.setSize(sf::Vector2f(40.f, 8.f));
    gun.setFillColor(sf::Color::Red);
    gun.setOrigin(0.f, 4.f);
}

void Player::update(float deltaTime) {
    velocity.x = 0.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) velocity.x = -movementSpeed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) velocity.x = movementSpeed;

    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::W)||sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) && isGrounded) {
        velocity.y = -jumpForce;
        isGrounded = false;
    }

    velocity.y += gravity * deltaTime;

    position.x += velocity.x * deltaTime;

    if (position.x < 0.f) position.x = 0.f;
    else if (position.x > 1378.f - 50.f) position.x = 1378.f - 50.f;

    position.y += velocity.y * deltaTime;
    shape.setPosition(position);

    gun.setPosition(getCenter());
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(shape); 
    window.draw(gun);   
}

void Player::aim(sf::Vector2f mousePos) {
    sf::Vector2f playerCenter = getCenter();

    float dx = mousePos.x - playerCenter.x;
    float dy = mousePos.y - playerCenter.y;

    float angle = std::atan2(dy, dx) * 180.f / 3.14159265f;
    gun.setRotation(angle);
}

sf::Vector2f Player::getCenter() const {
    return sf::Vector2f(position.x + 25.f, position.y + 25.f);
}

void Player::setGrounded(bool state) {
    isGrounded = state;
}

void Player::resolveCollision(const sf::FloatRect& platformBounds) {
    sf::FloatRect playerBounds = getBounds();
    if (velocity.y > 0.f && (playerBounds.top < platformBounds.top)) {
        position.y = platformBounds.top - playerBounds.height;
        velocity.y = 0.f;
        isGrounded = true;
        shape.setPosition(position);
    }
}