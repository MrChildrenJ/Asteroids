#include "game.h"
#include "window.h"
#include <fstream>
#include <algorithm>

Game::Game()
    : window("Asteroids", Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT)),
      player(window.GetWindowSize().x / 2.0f, window.GetWindowSize().y / 2.0f) {
  // Set up score font and text
  font.loadFromFile("IosevkaNerdFont-Bold.ttf");
  scoreText.setFont(font);
  scoreText.setCharacterSize(24);
  scoreText.setFillColor(sf::Color::White);
  score = 0;
  
  // Set up game over text
  gameOverText.setFont(font);
  gameOverText.setCharacterSize(36);
  gameOverText.setFillColor(sf::Color::Red);
  gameOverText.setString("GAME OVER\n\nPress SPACE to continue");
  gameOverText.setPosition(WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT / 2 - 100);
  
  // Set up name input text
  nameInputText.setFont(font);
  nameInputText.setCharacterSize(24);
  nameInputText.setFillColor(sf::Color::White);
  nameInputText.setPosition(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2);
  
  // Set up leaderboard text
  leaderboardText.setFont(font);
  leaderboardText.setCharacterSize(20);
  leaderboardText.setFillColor(sf::Color::White);
  leaderboardText.setPosition(50, 50);
  
  loadLeaderboard();

  // initialize the asteroids
  vector<string> textureFiles = {"images/asteroid_04.png",
                                 "images/asteroid_05.png",
                                 "images/asteroid_06.png"};
  textures = loadTextures(textureFiles);

  Asteroid::initializeAsteroids(asteroids, textures, window.GetWindowSize(),
                                NUMBER_OF_INITIAL_ASTEROIDS);

  clock.restart();
}

void Game::run() {
  while (!window.IsDone()) {
    float deltaTime = clock.restart().asSeconds();
    Event event;

    // detection for closing window
    while (window.GetWindow().pollEvent(event)) {
      if (event.type == sf::Event::Closed ||
          event.type == sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        window.Destroy();

      // Handle input based on game state
      switch (gameState) {
        case GameState::PLAYING:
          player.handleInput(event);
          break;
        case GameState::GAME_OVER:
          handleGameOverInput(event);
          break;
        case GameState::NAME_INPUT:
          handleNameInput(event);
          break;
        case GameState::LEADERBOARD:
          handleLeaderboardInput(event);
          break;
      }
    }

    // Update game logic only when playing
    if (gameState == GameState::PLAYING) {
      player.update(lasers, deltaTime);
      player.updateInvincibility();

      // Check ship-asteroid collision with HP system
      for (size_t i = 0; i < asteroids.size(); ++i) {
        if (player.checkCollision(asteroids[i]) && player.canTakeDamage()) {
          player.takeDamage();
          cout << "Player hit! HP remaining: " << player.getHp() << endl;
          
          // Handle asteroid splitting/removal on collision
          if (asteroids[i].isSmall()) {
            // Small asteroid disappears
            asteroids.erase(asteroids.begin() + i);
            --i; // Adjust index after removal
          } else {
            // Large asteroid splits into two smaller ones
            breakApart(asteroids, i, textures);
            ++i; // Skip the next asteroid since we just added one
          }
          
          if (!player.isAlive()) {
            gameState = GameState::GAME_OVER;
            break;
          }
        }
      }

      // Only spawn asteroids if player is alive
      if (player.isAlive()) {
        asteroidSpawnTimer += deltaTime;
        if (asteroidSpawnTimer >= 3.0f) {
          Asteroid::initializeAsteroids(asteroids, textures, window.GetWindowSize(), 1);
          asteroidSpawnTimer = 0.0f;
        }

        // let asteroids move and rotate
        updateAsteroids(asteroids, deltaTime, window.GetWindowSize());

        // update lasers
        for (auto &laser : lasers)
          laser.update(deltaTime);

        // laser-asteroid collision detection
        for (size_t i = 0; i < asteroids.size(); ++i)
          for (auto it = lasers.begin(); it != lasers.end();) {
            if (asteroids[i].isHitByLaser(it->getSprite())) {
              breakApart(asteroids, i, textures);
              score += 100;
              it = lasers.erase(it);
            } else
              ++it;
          }

        lasers.erase(remove_if(lasers.begin(), lasers.end(),
                               [this](const Laser &laser) {
                                 return laser.isOutsideWindow(window.GetWindowSize());
                               }),
                     lasers.end());
      }
    }

    // Render based on game state
    window.BeginDraw();

    switch (gameState) {
      case GameState::PLAYING:
        window.RenderAsteroids(asteroids);
        if (player.isAlive() && player.shouldDraw()) {
          window.Draw(player.getSprite());
        }
        for (auto &laser : lasers)
          window.Draw(laser.getSprite());

        scoreText.setString("SCORE: " + to_string(score) + "  HP: " + to_string(player.getHp()));
        window.Draw(scoreText);
        break;

      case GameState::GAME_OVER:
        window.Draw(gameOverText);
        break;

      case GameState::NAME_INPUT:
        nameInputText.setString("Enter name: " + playerName + string(3 - playerName.length(), '_'));
        window.Draw(nameInputText);
        break;

      case GameState::LEADERBOARD:
        window.Draw(leaderboardText);
        break;
    }

    window.EndDraw();
  }
}

void Game::handleGameOverInput(Event &event) {
  if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space) {
    gameState = GameState::NAME_INPUT;
    playerName = "";
    nameInputTimer.restart();
  }
}

void Game::handleNameInput(Event &event) {
  if (event.type == Event::TextEntered && nameInputTimer.getElapsedTime().asSeconds() > nameInputDelay) {
    char inputChar = static_cast<char>(event.text.unicode);
    
    // Only allow alphanumeric characters and spaces, exclude control characters
    if (playerName.length() < 3 && 
        ((inputChar >= 'A' && inputChar <= 'Z') ||
         (inputChar >= 'a' && inputChar <= 'z') ||
         (inputChar >= '0' && inputChar <= '9') ||
         inputChar == ' ')) {
      playerName += inputChar;
    }
  }
  else if (event.type == Event::KeyPressed) {
    if (event.key.code == Keyboard::BackSpace && !playerName.empty()) {
      playerName.pop_back();
    }
    else if (event.key.code == Keyboard::Enter && playerName.length() == 3) {
      saveScore();
      gameState = GameState::LEADERBOARD;
    }
  }
}

void Game::handleLeaderboardInput(Event &event) {
  if (event.type == Event::KeyPressed) {
    if (event.key.code == Keyboard::R) {
      resetGame();
    }
    else if (event.key.code == Keyboard::Q) {
      window.Destroy();
    }
  }
}

void Game::saveScore() {
  leaderboard.push_back(make_pair(playerName, score));
  sort(leaderboard.begin(), leaderboard.end(), 
       [](const pair<string, int> &a, const pair<string, int> &b) {
         return a.second > b.second;
       });
  
  if (leaderboard.size() > 10) {
    leaderboard.resize(10);
  }
  
  saveLeaderboard();
}

void Game::loadLeaderboard() {
  leaderboard.clear();
  ifstream file("leaderboard.txt");
  if (file.is_open()) {
    string name;
    int scoreValue;
    while (file >> name >> scoreValue) {
      leaderboard.push_back(make_pair(name, scoreValue));
    }
    file.close();
  }
}

void Game::saveLeaderboard() {
  ofstream file("leaderboard.txt");
  if (file.is_open()) {
    for (const auto &entry : leaderboard) {
      file << entry.first << " " << entry.second << "\n";
    }
    file.close();
  }
  
  string leaderboardStr = "LEADERBOARD\n\n";
  for (size_t i = 0; i < leaderboard.size(); ++i) {
    leaderboardStr += to_string(i + 1) + ". " + leaderboard[i].first + " - " + to_string(leaderboard[i].second) + "\n";
  }
  leaderboardStr += "\nPress R to restart, Q to quit";
  leaderboardText.setString(leaderboardStr);
}

void Game::resetGame() {
  score = 0;
  gameState = GameState::PLAYING;
  playerName = "";
  
  // Reset player to starting position and reset HP
  player.setPosition(window.GetWindowSize().x / 2.0f, window.GetWindowSize().y / 2.0f);
  player.resetHealth();
  
  asteroids.clear();
  lasers.clear();
  
  Asteroid::initializeAsteroids(asteroids, textures, window.GetWindowSize(), NUMBER_OF_INITIAL_ASTEROIDS);
  
  asteroidSpawnTimer = 0.0f;
  clock.restart();
}