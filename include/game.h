#ifndef game_hpp
#define game_hpp

#include "asteroid.h"
#include "laser.h"
#include "ship.h"
#include "window.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <stdio.h>
#include <vector>
#define NUMBER_OF_INITIAL_ASTEROIDS 10

using namespace sf;
using namespace std;

enum class GameState {
  PLAYING,
  GAME_OVER,
  NAME_INPUT,
  LEADERBOARD
};

class Game {
public:
  Game();
  void initializeGame();
  void run();

private:
  Font font;
  Text scoreText;
  Text gameOverText;
  Text nameInputText;
  Text leaderboardText;
  int score;
  GameState gameState = GameState::PLAYING;
  
  helper::Window window;
  Ship player;
  vector<unique_ptr<Texture>> textures;
  vector<Asteroid> asteroids;
  vector<Laser> lasers;
  Clock clock;
  float asteroidSpawnTimer = 0.0f;
  
  string playerName = "";
  vector<pair<string, int>> leaderboard;
  Clock nameInputTimer;
  const float nameInputDelay = 0.2f;
  
  void handleGameOverInput(Event &event);
  void handleNameInput(Event &event);
  void handleLeaderboardInput(Event &event);
  void saveScore();
  void loadLeaderboard();
  void saveLeaderboard();
  void resetGame();
};

#endif /* game_hpp */