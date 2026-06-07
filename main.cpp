#include <SFML/Graphics.hpp>
#include <iostream>

// Podział na ekrany w grze
enum GameState {
    MENU,
    PLAY,
    AUTHORS
};

int main() {
    // Inicjalizacja okna
    sf::RenderWindow window(sf::VideoMode(1378, 784), "Corpse Jump - Menu");
    window.setFramerateLimit(60);

    //  Ładowanie czcionki
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Blad: Nie mozna wczytac czcionki!" << std::endl;
        return -1;
    }

    // Stan początkowy gry
    GameState currentState = MENU;
    // Ładowanie grafiki tła
    sf::Texture menuTexture;
    if (!menuTexture.loadFromFile("authors.png")) {
        std::cerr << "Blad wczytywania grafiki menu!" << std::endl;
        return -1;
    }
    sf::Sprite background(menuTexture);


    // Konfiguracja przycisków
    // Przycisk PLAY
    sf::RectangleShape playButton(sf::Vector2f(290.f, 80.f));
    playButton.setPosition(545.f, 330.f);
    playButton.setFillColor(sf::Color(100, 100, 100));

    sf::Text playText("PLAY", font, 30);
    playText.setFillColor(sf::Color::White);
    playText.setPosition(650.f, 350.f);

    // Przycisk AUTHORS
    sf::RectangleShape authorsButton(sf::Vector2f(290.f, 80.f));
    authorsButton.setPosition(545.f, 430.f);
    authorsButton.setFillColor(sf::Color(100, 100, 100));

    sf::Text authorsText("AUTHORS", font, 30);
    authorsText.setFillColor(sf::Color::White);
    authorsText.setPosition(620.f, 450.f);

    // Przycisk EXIT
    sf::RectangleShape exitButton(sf::Vector2f(290.f, 80.f));
    exitButton.setPosition(545.f, 530.f);
    exitButton.setFillColor(sf::Color(100, 100, 100));

    sf::Text exitText("EXIT", font, 30);
    exitText.setFillColor(sf::Color::White);
    exitText.setPosition(655.f, 550.f);

    // Teksty dla pozostałych ekranów gry
    sf::Text gameText("Wcisnij ESC, aby wrocic do Menu", font, 30);
    gameText.setPosition(455.f, 450.f);

    sf::Text authorsInfo("\t\tBartosz Gacek\n\t\tIwo Galezewski\nWcisnij ESC, aby wrocic", font, 40);
    authorsInfo.setPosition(450.f, 400.f);


    // Główna pętla gry
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // Zamykanie okna krzyżykiem
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Logika dla stanu MENU
            if (currentState == MENU) {
                // Pobieranie pozycji kursora względem okna
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

                // Efekt najechania myszką
                if (playButton.getGlobalBounds().contains(mousePosF)) playButton.setFillColor(sf::Color(150, 150, 150));
                else playButton.setFillColor(sf::Color(100, 100, 100));

                if (authorsButton.getGlobalBounds().contains(mousePosF)) authorsButton.setFillColor(sf::Color(150, 150, 150));
                else authorsButton.setFillColor(sf::Color(100, 100, 100));

                if (exitButton.getGlobalBounds().contains(mousePosF)) exitButton.setFillColor(sf::Color(150, 150, 150));
                else exitButton.setFillColor(sf::Color(100, 100, 100));

                // Reakcja na kliknięcie
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (playButton.getGlobalBounds().contains(mousePosF)) {
                            currentState = PLAY; // Przejście do gry
                        }
                        if (authorsButton.getGlobalBounds().contains(mousePosF)) {
                            currentState = AUTHORS; // Przejście do authors
                        }
                        if (exitButton.getGlobalBounds().contains(mousePosF)) {
                            window.close(); // Wyjście z gry
                        }
                    }
                }
                // Wyjście z gry za pomocą ESC w menu
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Escape) {
                        window.close();}}
            }
            // Powrót do menu głównego z innych ekranów za pomocą ESC
            else if (currentState == PLAY || currentState == AUTHORS) {
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                    currentState = MENU; // Powrót do menu po wciśnięciu ESC
                }
            }
        }

        // Rysowanie klatek
        window.clear(sf::Color(40, 45, 50));

        window.draw(background);

        if (currentState == MENU) {
            window.draw(playButton);
            window.draw(playText);

            window.draw(authorsButton);
            window.draw(authorsText);

            window.draw(exitButton);
            window.draw(exitText);
        }
        else if (currentState == PLAY) {
            window.draw(gameText);
        }
        else if (currentState == AUTHORS) {
            window.draw(authorsInfo);
        }

        window.display();
    }

    return 0;
}