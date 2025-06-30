#pragma once
#include "laser.h"
#include "window.h"
#include <SFML/Graphics.hpp>

class Asteroid;

using namespace sf;
using namespace std;

struct KeyStatus {
  bool Up;
  bool Left;
  bool Right;
  bool Space;

  KeyStatus(bool initial)
      : Up(initial), Left(initial), Right(initial), Space(initial) {}
};

class Ship : public sf::Drawable, public sf::Transformable {
private:
  const uint32_t RATE_OF_FIRE = 400;
  const float TURN_SPEED = 0.033f;
  const float ACCELERATION = 150.f; // TODO tune this

  sf::Texture m_texture;
  sf::Sprite sprite;

  sf::Clock cooldown;

  KeyStatus KeyDown;

  float m_velocity;
  Vector2f m_direction;

  int hitCount = 0;
  bool isBlinking = false;
  Clock blinkClock;
  const float blinkDuration = 1.0f;
  
  int hp = 3;
  const int maxHp = 3;
  Clock damageCooldown;
  const float damageInterval = 1.0f;
  
  bool isInvincible = false;
  Clock invincibilityClock;
  const float invincibilityDuration = 2.0f;
  Clock flashClock;
  const float flashInterval = 0.1f;
  bool flashVisible = true;
  const float shimmerDuration = 1.0f;

  virtual void draw(sf::RenderTarget &target,
                    sf::RenderStates states) const override {
    states.transform *= getTransform();
    if (!isInvincible || flashVisible) {
      target.draw(sprite, states);
    }
  }

public:
  Ship(float x, float y);
  ~Ship() {}
  sf::Sprite &getSprite() { return sprite; }
  void handleInput(Event &e);
  void update(vector<Laser> &lasers, float &deltaTime);

  Vector2f getDirection() const { return m_direction; }
  Vector2f getNosePosition() const;

  bool checkCollision(const Asteroid &asteroid);
  FloatRect getGlobalBounds() const;
  
  int getHp() const { return hp; }
  void takeDamage() { 
    if (hp > 0 && !isInvincible) {
      hp--;
      isInvincible = true;
      invincibilityClock.restart();
      flashClock.restart();
      damageCooldown.restart();
    }
  }
  bool isAlive() const { return hp > 0; }
  bool canTakeDamage() const { return !isInvincible; }
  void resetHealth() { 
    hp = maxHp; 
    isInvincible = false;
    flashVisible = true;
    damageCooldown.restart(); 
  }
  void updateInvincibility() {
    if (isInvincible) {
      float elapsedTime = invincibilityClock.getElapsedTime().asSeconds();
      
      if (elapsedTime >= invincibilityDuration) {
        isInvincible = false;
        flashVisible = true;
      } else if (elapsedTime < shimmerDuration && flashClock.getElapsedTime().asSeconds() >= flashInterval) {
        flashVisible = !flashVisible;
        flashClock.restart();
      } else if (elapsedTime >= shimmerDuration) {
        flashVisible = true; // Stop flashing after 1 second, but stay invincible
      }
    }
  }
  bool shouldDraw() const { return !isInvincible || flashVisible; }
};