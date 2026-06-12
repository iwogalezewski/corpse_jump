#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <fstream> 
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

    std::vector<std::unique_ptr<GameObject>> gameObjects;

    sf::Font font;
    sf::Texture menuTexture;
    sf::Sprite background;

    sf::RectangleShape playButton;
    sf::RectangleShape authorsButton;
    sf::RectangleShape exitButton;

    sf::Text playText;
    sf::Text authorsText;
    sf::Text exitText;
    sf::Text gameText;
    sf::Text authorsInfo;

    sf::Text timerText;
    sf::Text bestScoreText;
    float currentSurvivalTime = 0.f;
    float bestSurvivalTime = 0.f;


    void loadBestScore();
    void saveBestScore();
    void checkAndSaveScore();

    float spawnTimer = 0.f;
    float nextSpawnTime = 3.0f;
    int zombieKills = 0;

    void processEvents();
    void update(float deltaTime);
    void render();
    void initUI();

public:
    Game();
    void run();
};