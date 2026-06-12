#include "Game.h"
#include "Player.h"
#include "Platform.h"
#include "Bullet.h"
#include "TankZombiee.h"
#include "FastZombie.h"
#include <cstdlib>
#include <ctime>
#include <string>
#include <algorithm>
#include <cmath>
Game::Game() : window(sf::VideoMode(1378, 784), "Corpse Jump"), currentState(GameState::MENU) {
    window.setFramerateLimit(60);
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    if (!font.loadFromFile("arial.ttf")) std::cerr << "Blad: Nie mozna wczytac czcionki!" << std::endl;
    if (!menuTexture.loadFromFile("authors.png")) std::cerr << "Blad wczytywania grafiki!" << std::endl;
    background.setTexture(menuTexture);

    // Wczytanie najlepszego wyniku 
    loadBestScore();
    initUI();

    //MAPA
    gameObjects.push_back(std::make_unique<Platform>(-500.f, 700.f, 2500.f, 100.f));
    gameObjects.push_back(std::make_unique<Platform>(200.f, 500.f, 250.f, 30.f));
    gameObjects.push_back(std::make_unique<Platform>(800.f, 400.f, 250.f, 30.f));
    gameObjects.push_back(std::make_unique<Platform>(450.f, 250.f, 200.f, 30.f));

    gameObjects.push_back(std::make_unique<Player>(100.f, 600.f));
}

void Game::initUI() {
    //BEST SCORE 
    bestScoreText.setFont(font);
    bestScoreText.setString("Najlepszy czas: " + std::to_string(static_cast<int>(bestSurvivalTime)) + "s");
    bestScoreText.setCharacterSize(40);
    bestScoreText.setFillColor(sf::Color::Yellow);

    sf::FloatRect bsRect = bestScoreText.getLocalBounds();
    bestScoreText.setOrigin(bsRect.left + bsRect.width / 2.0f, bsRect.top + bsRect.height / 2.0f);
    bestScoreText.setPosition(1378.f / 2.0f, 650.f);

    //AKTUALNY CZAS
    timerText.setFont(font);
    timerText.setString("0s");
    timerText.setCharacterSize(50);
    timerText.setFillColor(sf::Color::White);

    //PLAY 
    playButton.setSize(sf::Vector2f(290.f, 80.f));
    playButton.setPosition(545.f, 330.f);
    playButton.setFillColor(sf::Color(100, 100, 100));

    playText.setFont(font);
    playText.setString("GRAJ");
    playText.setCharacterSize(30);
    playText.setFillColor(sf::Color::White);

    sf::FloatRect playRect = playText.getLocalBounds();
    playText.setOrigin(playRect.left + playRect.width / 2.0f, playRect.top + playRect.height / 2.0f);
    playText.setPosition(playButton.getPosition().x + playButton.getSize().x / 2.0f,
        playButton.getPosition().y + playButton.getSize().y / 2.0f);

    //(AUTORZY
    authorsButton.setSize(sf::Vector2f(290.f, 80.f));
    authorsButton.setPosition(545.f, 430.f);
    authorsButton.setFillColor(sf::Color(100, 100, 100));

    authorsText.setFont(font);
    authorsText.setString("AUTORZY");
    authorsText.setCharacterSize(30);
    authorsText.setFillColor(sf::Color::White);

    sf::FloatRect authRect = authorsText.getLocalBounds();
    authorsText.setOrigin(authRect.left + authRect.width / 2.0f, authRect.top + authRect.height / 2.0f);
    authorsText.setPosition(authorsButton.getPosition().x + authorsButton.getSize().x / 2.0f,
        authorsButton.getPosition().y + authorsButton.getSize().y / 2.0f);

    //EXIT
    exitButton.setSize(sf::Vector2f(290.f, 80.f));
    exitButton.setPosition(545.f, 530.f);
    exitButton.setFillColor(sf::Color(100, 100, 100));

    exitText.setFont(font);
    exitText.setString("WYJSCIE");
    exitText.setCharacterSize(30);
    exitText.setFillColor(sf::Color::White);

    sf::FloatRect exitRect = exitText.getLocalBounds();
    exitText.setOrigin(exitRect.left + exitRect.width / 2.0f, exitRect.top + exitRect.height / 2.0f);
    exitText.setPosition(exitButton.getPosition().x + exitButton.getSize().x / 2.0f,
        exitButton.getPosition().y + exitButton.getSize().y / 2.0f);

    gameText.setFont(font);
    gameText.setString("ESC - Menu");
    gameText.setCharacterSize(20);
    gameText.setPosition(20.f, 20.f);

    authorsInfo.setFont(font);
    authorsInfo.setString("\t\tBartosz Gacek\n\t\tIwo Galezewski\nWcisnij ESC, aby wrocic");
    authorsInfo.setCharacterSize(40);
    authorsInfo.setPosition(450.f, 400.f);
}

void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (currentState == GameState::MENU) {
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePosF(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

                if (playButton.getGlobalBounds().contains(mousePosF)) {
                    currentState = GameState::PLAY;
                    currentSurvivalTime = 0.f; // Reset licznika
                }
                if (authorsButton.getGlobalBounds().contains(mousePosF)) currentState = GameState::AUTHORS;
                if (exitButton.getGlobalBounds().contains(mousePosF)) window.close();
            }
     
        }
        else if (currentState == GameState::PLAY) {
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                checkAndSaveScore(); // Sprawdzenie i zapis 
            }
           

            //STRZELANIA
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePosF = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                Player* playerPtr = nullptr;
                for (auto& obj : gameObjects) {
                    if (auto p = dynamic_cast<Player*>(obj.get())) {
                        playerPtr = p;
                        break;
                    }
                }

                if (playerPtr) {
                    sf::Vector2f playerCenter = playerPtr->getCenter();
                    float dx = mousePosF.x - playerCenter.x;
                    float dy = mousePosF.y - playerCenter.y;
                    float length = std::sqrt(dx * dx + dy * dy);

                    if (length != 0) {

                        sf::Vector2f dir(dx / length, dy / length);
                        int dmg = 1;
                        if (playerPtr->hasSpecialAmmo()){
                            dmg = 2;
                            playerPtr->useSpecialAmmo();
                        }
                        gameObjects.push_back(std::make_unique<Bullet>(playerPtr->getCenter().x, playerPtr->getCenter().y, dir, dmg));
                    }
                }
            }
        }
        else if (currentState == GameState::AUTHORS) {
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) currentState = GameState::MENU;
        }
    }
}

void Game::update(float deltaTime) {
    if (currentState == GameState::MENU) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePosF = window.mapPixelToCoords(mousePos);


        // Podœwietlanie przycisków
        playButton.setFillColor(playButton.getGlobalBounds().contains(mousePosF) ? sf::Color(150, 150, 150) : sf::Color(100, 100, 100));
        authorsButton.setFillColor(authorsButton.getGlobalBounds().contains(mousePosF) ? sf::Color(150, 150, 150) : sf::Color(100, 100, 100));
        exitButton.setFillColor(exitButton.getGlobalBounds().contains(mousePosF) ? sf::Color(150, 150, 150) : sf::Color(100, 100, 100));
    }
    else if (currentState == GameState::PLAY) {

        // Aktualizacja timera
        currentSurvivalTime += deltaTime;
        timerText.setString(std::to_string(static_cast<int>(currentSurvivalTime)) + "s");
        sf::FloatRect timerRect = timerText.getLocalBounds();
        timerText.setOrigin(timerRect.left + timerRect.width / 2.0f, timerRect.top + timerRect.height / 2.0f);
        timerText.setPosition(1378.f / 2.0f, 50.f);

        // Zombie spawner
        spawnTimer += deltaTime;
        if (spawnTimer >= nextSpawnTime) {
            spawnTimer = 0.f;

            nextSpawnTime = 1.5f + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / 3.0f));

            // Losowanie strony
            float spawnX = (std::rand() % 2 == 0) ? -100.f : 1450.f;
            float spawnY = 600.f;
            // 60% - Zwyk³y, 30% - Szybki, 10% - Tank
            int chance = std::rand() % 100;

            if (chance < 60) {
                gameObjects.push_back(std::make_unique<Zombie>(spawnX, spawnY));
            }
            else if (chance < 90) {
                gameObjects.push_back(std::make_unique<FastZombie>(spawnX, spawnY));
            }
            else {
                gameObjects.push_back(std::make_unique<TankZombiee>(spawnX, spawnY));
            }
        }
        for (auto& obj : gameObjects) {
            obj->update(deltaTime);
        }
        Player* playerPtr = nullptr;
        for (auto& obj : gameObjects) {
            if (auto p = dynamic_cast<Player*>(obj.get())) {
                playerPtr = p;
                break;
            }
        }

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

        for (auto& obj : gameObjects) {
            if (auto player = dynamic_cast<Player*>(obj.get())) {
                player->aim(mousePosF);
            }
            else if (auto zombie = dynamic_cast<Zombie*>(obj.get())) {
                    zombie->moveTowards(playerPtr->getCenter().x);
                    //w strone gracza
                }
            }
        }

        // Ziemia/Platformy
        for (auto& obj1 : gameObjects) {
            if (auto player = dynamic_cast<Player*>(obj1.get())) {
                player->setGrounded(false);
                for (auto& obj2 : gameObjects) {
                    if (auto platform = dynamic_cast<Platform*>(obj2.get())) {
                        if (player->getBounds().intersects(platform->getBounds())) {
                            player->resolveCollision(platform->getBounds());
                        }
                    }
                }
            }
            else if (auto zombie = dynamic_cast<Zombie*>(obj1.get())) {
                zombie->setGrounded(false);
                for (auto& obj2 : gameObjects) {
                    if (auto platform = dynamic_cast<Platform*>(obj2.get())) {
                        if (zombie->getBounds().intersects(platform->getBounds())) {
                            zombie->resolveCollision(platform->getBounds());
                        }
                    }
                }
            }
        }

        // Pocisk i Zombie
        Player* playerPtr = nullptr;
        for (auto& obj : gameObjects) {
            if (auto p = dynamic_cast<Player*>(obj.get())) {
                playerPtr = p;
                break;
            }
        }

        for (auto& obj1 : gameObjects) {
            if (auto bullet = dynamic_cast<Bullet*>(obj1.get())) {
                if (bullet->isMarkedForDeletion()) continue;

                for (auto& obj2 : gameObjects) {
                    if (auto zombie = dynamic_cast<Zombie*>(obj2.get())) {
                        if (zombie->isMarkedForDeletion()) continue;
                        if (bullet->getBounds().intersects(zombie->getBounds())) {
                            zombie->takeDamage(bullet->getDamage());
                            if(zombie->isMarkedForDeletion()){
                                zombieKills++;
                                if(zombieKills % 10 == 0){
                                    playerPtr->addSpecialAmmo(5);
                                }
                            }

                            bullet->markForDeletion(); 
                            break; 
                        }
                    }
                }
            }
        }
        gameObjects.erase(
            std::remove_if(gameObjects.begin(), gameObjects.end(),
                [](const std::unique_ptr<GameObject>& obj) { return obj->isMarkedForDeletion(); }),
            gameObjects.end()
        );
    }

void Game::render() {
    window.clear(sf::Color(40, 45, 50));
    window.draw(background);

    if (currentState == GameState::MENU) {
        window.draw(bestScoreText); 


        window.draw(playButton);
        window.draw(playText);
        window.draw(authorsButton);
        window.draw(authorsText);
        window.draw(exitButton);
        window.draw(exitText);
    }
    else if (currentState == GameState::PLAY) {

        for (auto& obj : gameObjects) {
            obj->draw(window);
        }
        window.draw(gameText);  
        window.draw(timerText); 
    }
    else if (currentState == GameState::AUTHORS) {
        window.draw(authorsInfo);
    }

    window.display();
}

void Game::loadBestScore() {
    std::ifstream file("best_score.txt");
    if (file.is_open()) {
        file >> bestSurvivalTime;
        file.close();
    }
    else {
        bestSurvivalTime = 0.f; 
    }
}

    void Game::saveBestScore() {
        std::ofstream file("best_score.txt");
        if (file.is_open()) {
            file << bestSurvivalTime;
            file.close();
        }
    }

    void Game::checkAndSaveScore() {
        if (currentSurvivalTime > bestSurvivalTime) {
            bestSurvivalTime = currentSurvivalTime;
            saveBestScore();
            bestScoreText.setString("Najlepszy czas: " + std::to_string(static_cast<int>(bestSurvivalTime)) + "s");
            sf::FloatRect bsRect = bestScoreText.getLocalBounds();
            bestScoreText.setOrigin(bsRect.left + bsRect.width / 2.0f, bsRect.top + bsRect.height / 2.0f);
            bestScoreText.setPosition(1378.f / 2.0f, 650.f);
        }
    }