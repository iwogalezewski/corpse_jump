#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include "GameObject.h"

enum class GameState {
    MENU,
    PLAY,
    AUTHORS
};

class Game {
private:
    sf::RenderWindow window;
    GameState currentState;

    // Przechowywanie wszystkich obiektów w jednym kontenerze
    std::vector<std::unique_ptr<GameObject>> gameObjects;

    // Zasoby
    sf::Font font;
    sf::Texture menuTexture;
    sf::Sprite background;

    // Elementy UI
    sf::RectangleShape playButton;
    sf::RectangleShape authorsButton;
    sf::RectangleShape exitButton;

    sf::Text playText;
    sf::Text authorsText;
    sf::Text exitText;
    sf::Text gameText;
    sf::Text authorsInfo;

    void processEvents();
    void update(float deltaTime);
    void render();
    void initUI();

public:
    Game();
    void run();
};