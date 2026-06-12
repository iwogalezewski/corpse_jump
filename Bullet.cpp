#include "Bullet.h"

Bullet::Bullet(float startX, float startY, sf::Vector2f dir, int dmg)
    : GameObject(startX, startY), direction(dir), speed(800.f), damage(dmg) {
    // Ma³y ¿ó³ty kwadracik jako pocisk
    shape.setSize(sf::Vector2f(10.f, 10.f));
    shape.setFillColor(sf::Color::Yellow);
    shape.setOrigin(5.f, 5.f); // Œrodek obiektu
}

void Bullet::update(float deltaTime) {
    // Ruch w zadanym kierunku
    position.x += direction.x * speed * deltaTime;
    position.y += direction.y * speed * deltaTime;

    shape.setPosition(position);

    // Jeœli wyleci za ekran, oznacz do usuniêcia (¿eby nie zapchaæ pamiêci RAM)
    if (position.x < 0.f || position.x > 1378.f || position.y < 0.f || position.y > 784.f) {
        dead = true;
    }
}