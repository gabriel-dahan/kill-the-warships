#include "Game.h"

// CONSTRUCTORS / DESTRUCTORS
Game::Game() {
    this->initVariables();
    this->initWindow();
    this->initFonts();
    this->initText();
    this->initEnemies();
}

Game::~Game() {
    delete this->window;
}

sf::Vector2f getSpriteSize(sf::Sprite sprite) {
    return sf::Vector2f(
        sprite.getTexture()->getSize().x * sprite.getScale().x,
        sprite.getTexture()->getSize().y * sprite.getScale().y
    );
}

// ACCESSORS
const bool Game::isRunning() const {
    return this->window->isOpen();
}

const bool Game::getEndGame() const {
    return this->endGame;
}

// PRIVATE FUNCTIONS
void Game::initVariables() {
    this->window = nullptr;

    // Game logic
    this->endGame = false;
    this->points = 0;
    this->health = 15;
    this->enemySpawnTimerMax = 10.f;
    this->enemySpawnTimer = this->enemySpawnTimerMax;
    this->maxEnemies = 10;
    this->mouseHeld = false;
}

void Game::initFonts() {
    if (this->font.loadFromFile("./fonts/evilempire.ttf")) {
        std::cerr << "ERR::Game::initFonts : Failed to load font ./fonts/evilempire.ttf !" << std::endl; 
    }
}

void Game::initText() {
    this->uiText.setFont(this->font);
    this->uiText.setCharacterSize(15);
    this->uiText.setFillColor(sf::Color::White);
    this->uiText.setString("TextErr : None");
}

void Game::initWindow() {
    this->videoMode.width = 800;
    this->videoMode.height = 600;

    this->window = new sf::RenderWindow(this->videoMode, "SFML Test Game", sf::Style::Titlebar | sf::Style::Close);
    this->window->setPosition(sf::Vector2i(500, 100));

    this->window->setFramerateLimit(60);
}

void Game::initEnemies() {
    this->enemyTexture.loadFromFile("./img/enemy-sprite.png");
    this->enemy.setTexture(this->enemyTexture);
    sf::Vector2f targetSize(50.f, 50.f);
    this->enemy.setScale(
        targetSize.x / this->enemy.getLocalBounds().width,
        targetSize.y / this->enemy.getLocalBounds().height
    );
}

// FUNCTIONS
void Game::spawnEnemy() {
    sf::Vector2f enemySize(getSpriteSize(this->enemy));
    float randPosX = static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - enemySize.x));
    this->enemy.setPosition(randPosX, 0.f);

    // Spawn the enemy
    this->enemies.push_back(this->enemy);
}

void Game::pollEvents() {
    while (this->window->pollEvent(this->ev)) {
        switch (this->ev.type) {
            case sf::Event::Closed:
                this->window->close();
                break;
            case sf::Event::KeyPressed:
                if (this->ev.key.code == sf::Keyboard::Escape)
                    this->window->close();
                break;
        }
    }
}


void Game::updateText() {
    std::stringstream ss;
    ss << "Points : " << this->points << "\n" << "Health : " << this->health;

    this->uiText.setString(ss.str());
}

void Game::updateMousePositions() {
    this->mousePosWindow = sf::Mouse::getPosition(*this->window);
    this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

void Game::updateEnemies() {
    // Updating the timer for enemy spawning.
    if (this->enemies.size() < this->maxEnemies) {
        if (this->enemySpawnTimer >= this->enemySpawnTimerMax) {
            // Spawn an enemy and reset the timer.
            this->spawnEnemy();
            this->enemySpawnTimer = 0.f;
        }
        else
            this->enemySpawnTimer += 1.f;
    }

    // Move and update the enemies
    for (int i = 0; i < this->enemies.size(); i++) {
        bool deleted = false;

        this->enemies[i].move(0.f, 2.f);

        if (this->enemies[i].getPosition().y > this->window->getSize().y) {
            this->enemies.erase(this->enemies.begin() + i);
            this->health -= 1;
        }
    }

    // Check if clicked upon.
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (this->mouseHeld == false) {
            this->mouseHeld = true;
            bool deleted = false;
            for (int i = 0; i < this->enemies.size() && deleted == false; i++)
            {
                if (this->enemies[i].getGlobalBounds().contains(this->mousePosView)) {
                    // Delete the enemy
                    deleted = true;
                    this->enemies.erase(this->enemies.begin() + i);

                    // Gain points
                    this->points += 1;
                }
            }
        }
    }
    else 
        this->mouseHeld = false;
}

void Game::update() {
    
    this->pollEvents();
    
    if (this->endGame == false) {
        this->updateMousePositions();

        this->updateText();

        this->updateEnemies();
    }

    // End game condition
    if (this->health <= 0) 
        this->endGame = true;

    /*
        Get mouse position :
        - Relative to the screen :
        sf::Vector2i mousePos(sf::Mouse::getPosition()); 
        - Relative to the window :
        sf::Vector2i mousePos(sf::Mouse::getPosition(*this->window));

        std::cout << "Mouse position : " << mousePos.x << " " << mousePos.y << std::endl;
    */
}

void Game::renderText(sf::RenderTarget& target) {
    target.draw(this->uiText);
}

void Game::renderEnemies(sf::RenderTarget& target) {
    // Rendering all the enemies.
    for (auto &e : this->enemies) {
        target.draw(e);
    }
    
}

void Game::render() {
    this->window->clear();

    // Draw game objects.
    this->renderEnemies(*this->window);

    this->renderText(*this->window);

    this->window->display();
}