#include "Game.h"
#include "Player.h"
#include "Platform.h"
#include "Bullet.h"
#include <cmath>
#include <algorithm> 
#include "Zombie.h"

Game::Game() : window(sf::VideoMode(1378, 784), "Corpse Jump"), currentState(GameState::MENU) {
    window.setFramerateLimit(60);

    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Blad: Nie mozna wczytac czcionki 'arial.ttf'!" << std::endl;
    }
    if (!menuTexture.loadFromFile("authors.png")) {
        std::cerr << "Blad wczytywania grafiki 'authors.png'!" << std::endl;
    }
    background.setTexture(menuTexture);

    initUI();

    // TWORZENIE MAPY W KONTENERZE:
    gameObjects.push_back(std::make_unique<Platform>(0.f, 700.f, 1378.f, 100.f));
    gameObjects.push_back(std::make_unique<Platform>(200.f, 500.f, 250.f, 30.f));
    gameObjects.push_back(std::make_unique<Platform>(800.f, 400.f, 250.f, 30.f));
    gameObjects.push_back(std::make_unique<Platform>(450.f, 250.f, 200.f, 30.f));

    gameObjects.push_back(std::make_unique<Player>(100.f, 600.f));


    gameObjects.push_back(std::make_unique<Zombie>(1000.f, 600.f));
    gameObjects.push_back(std::make_unique<Zombie>(1200.f, 600.f));
}

void Game::initUI() {
    // ---- PRZYCISK PLAY ----
    playButton.setSize(sf::Vector2f(290.f, 80.f));
    playButton.setPosition(545.f, 330.f);
    playButton.setFillColor(sf::Color(100, 100, 100));

    playText.setFont(font);
    playText.setString("PLAY");
    playText.setCharacterSize(30);
    playText.setFillColor(sf::Color::White);

    // Dynamiczne centrowanie tekstu na przycisku
    sf::FloatRect playRect = playText.getLocalBounds();
    playText.setOrigin(playRect.left + playRect.width / 2.0f, playRect.top + playRect.height / 2.0f);
    playText.setPosition(playButton.getPosition().x + playButton.getSize().x / 2.0f,
        playButton.getPosition().y + playButton.getSize().y / 2.0f);

    // ---- PRZYCISK AUTHORS ----
    authorsButton.setSize(sf::Vector2f(290.f, 80.f));
    authorsButton.setPosition(545.f, 430.f);
    authorsButton.setFillColor(sf::Color(100, 100, 100));

    authorsText.setFont(font);
    authorsText.setString("AUTHORS");
    authorsText.setCharacterSize(30);
    authorsText.setFillColor(sf::Color::White);

    sf::FloatRect authRect = authorsText.getLocalBounds();
    authorsText.setOrigin(authRect.left + authRect.width / 2.0f, authRect.top + authRect.height / 2.0f);
    authorsText.setPosition(authorsButton.getPosition().x + authorsButton.getSize().x / 2.0f,
        authorsButton.getPosition().y + authorsButton.getSize().y / 2.0f);

    // ---- PRZYCISK EXIT ----
    exitButton.setSize(sf::Vector2f(290.f, 80.f));
    exitButton.setPosition(545.f, 530.f);
    exitButton.setFillColor(sf::Color(100, 100, 100));

    exitText.setFont(font);
    exitText.setString("EXIT");
    exitText.setCharacterSize(30);
    exitText.setFillColor(sf::Color::White);

    sf::FloatRect exitRect = exitText.getLocalBounds();
    exitText.setOrigin(exitRect.left + exitRect.width / 2.0f, exitRect.top + exitRect.height / 2.0f);
    exitText.setPosition(exitButton.getPosition().x + exitButton.getSize().x / 2.0f,
        exitButton.getPosition().y + exitButton.getSize().y / 2.0f);

    // ---- TEKSTY INFORMACYJNE ----
    gameText.setFont(font);
    gameText.setString("Wcisnij ESC, aby wrocic do Menu\nSterowanie: A, D, W / Spacja - skok");
    gameText.setCharacterSize(30);
    gameText.setPosition(/*45*/5.f, 50.f);

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
                sf::Vector2f mousePosF = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

                if (playButton.getGlobalBounds().contains(mousePosF)) currentState = GameState::PLAY;
                if (authorsButton.getGlobalBounds().contains(mousePosF)) currentState = GameState::AUTHORS;
                if (exitButton.getGlobalBounds().contains(mousePosF)) window.close();
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
        }
        else if (currentState == GameState::PLAY) {
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                currentState = GameState::MENU;
            }

            // ---- OBS£UGA STRZELANIA ----
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePosF = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

                // Szukamy gracza w kontenerze, ¿eby wiedzieæ sk¹d wystrzeliæ
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
                        // Tworzymy znormalizowany wektor kierunku (o d³ugoœci 1)
                        sf::Vector2f dir(dx / length, dy / length);
                        // Dodajemy nowy pocisk do naszego g³ównego kontenera!
                        gameObjects.push_back(std::make_unique<Bullet>(playerCenter.x, playerCenter.y, dir));
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
        playButton.setFillColor(playButton.getGlobalBounds().contains(mousePosF) ? sf::Color(150, 150, 150) : sf::Color(100, 100, 100));
        authorsButton.setFillColor(authorsButton.getGlobalBounds().contains(mousePosF) ? sf::Color(150, 150, 150) : sf::Color(100, 100, 100));
        exitButton.setFillColor(exitButton.getGlobalBounds().contains(mousePosF) ? sf::Color(150, 150, 150) : sf::Color(100, 100, 100));
    }
    else if (currentState == GameState::PLAY) {

        // 1. Zaktualizuj fizykê i timer wszystkich obiektów
        for (auto& obj : gameObjects) {
            obj->update(deltaTime);
        }

        // 2. ZnajdŸ wskaŸnik na gracza 
        Player* playerPtr = nullptr;
        for (auto& obj : gameObjects) {
            if (auto p = dynamic_cast<Player*>(obj.get())) {
                playerPtr = p;
                break;
            }
        }

        // 3. AI Zombie i celowanie gracza
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePosF = window.mapPixelToCoords(mousePos);
        for (auto& obj : gameObjects) {
            if (auto player = dynamic_cast<Player*>(obj.get())) {
                player->aim(mousePosF);
            }
            else if (auto zombie = dynamic_cast<Zombie*>(obj.get())) {
                if (playerPtr) {
                    // Zombiak idzie w stronê wspó³rzêdnej X gracza
                    zombie->moveTowards(playerPtr->getCenter().x);
                }
            }
        }

        // 4. Kolizje ze œrodowiskiem (Ziemia/Platformy)
        for (auto& obj1 : gameObjects) {
            if (auto player = dynamic_cast<Player*>(obj1.get())) {
                player->setGrounded(false);
                for (auto& obj2 : gameObjects) {
                    if (auto platform = dynamic_cast<Platform*>(obj2.get())) {
                        if (player->getBounds().intersects(platform->getBounds())) player->resolveCollision(platform->getBounds());
                    }
                }
            }
            else if (auto zombie = dynamic_cast<Zombie*>(obj1.get())) {
                zombie->setGrounded(false);
                for (auto& obj2 : gameObjects) {
                    if (auto platform = dynamic_cast<Platform*>(obj2.get())) {
                        if (zombie->getBounds().intersects(platform->getBounds())) zombie->resolveCollision(platform->getBounds());
                    }
                }
            }
        }

        // 5. Kolizje bojowe: Pocisk vs Zombie
        for (auto& obj1 : gameObjects) {
            if (auto bullet = dynamic_cast<Bullet*>(obj1.get())) {
                if (bullet->isMarkedForDeletion()) continue; // Nie sprawdzaj pocisków, które ju¿ w coœ uderzy³y w tej klatce

                for (auto& obj2 : gameObjects) {
                    if (auto zombie = dynamic_cast<Zombie*>(obj2.get())) {
                        if (zombie->isMarkedForDeletion()) continue;

                        // Jeœli dosz³o do przeciêcia (trafienie)
                        if (bullet->getBounds().intersects(zombie->getBounds())) {
                            zombie->takeDamage(1);     // Zombiak traci 1 HP
                            bullet->markForDeletion(); // Pocisk zostaje zniszczony
                            break; // Przerywamy pêtlê, bo pocisk nie mo¿e uderzyæ dwóch zombie na raz
                        }
                    }
                }
            }
        }

        // 6. Bezpieczne usuwanie martwych obiektów z pamiêci RAM (Zabite zombie i wystrzelone pociski)
        gameObjects.erase(
            std::remove_if(gameObjects.begin(), gameObjects.end(),
                [](const std::unique_ptr<GameObject>& obj) { return obj->isMarkedForDeletion(); }),
            gameObjects.end()
        );
    }
}

void Game::render() {
    window.clear(sf::Color(40, 45, 50));
    window.draw(background);

    if (currentState == GameState::MENU) {
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
    }
    else if (currentState == GameState::AUTHORS) {
        window.draw(authorsInfo);
    }

    window.display();
}