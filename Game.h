#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <vector>
#include <sstream>
#include <ctime>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

/*
    Wrapper classe - GAME Engine
*/

class Game {

private:
    // Varibales
        // Window
    sf::RenderWindow* window;
    sf::VideoMode videoMode;
    sf::Event ev;

        // Mouse positions
    sf::Vector2i mousePosWindow;
    sf::Vector2f mousePosView;

        // Resources
    sf::Font font;

        // Text
    sf::Text uiText;

        // Game logic
    bool endGame;
    unsigned points;
    int health;
    float enemySpawnTimer;
    float enemySpawnTimerMax;
    int maxEnemies;
    bool mouseHeld;

        // Game objects
    std::vector<sf::Sprite> enemies;
    sf::Texture enemyTexture;
    sf::Sprite enemy;

    // Functions
    void initVariables();
    void initFonts();
    void initText();
    void initWindow();
    void initEnemies();

public:
    // Constructors / Destructors
    Game();
    virtual ~Game();

    // Accessors
    const bool isRunning() const;
    const bool getEndGame() const;

    // Functions
    void spawnEnemy();

    void pollEvents();

    void updateMousePositions();
    void updateText();
    void updateEnemies();
    void update();
    
    void renderText(sf::RenderTarget& target);
    void renderEnemies(sf::RenderTarget& target);
    void render();
};

#endif