#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <stdio.h>
#include <vector>

using namespace sf;
using namespace std;

class Laser {
public:
  Laser(const Vector2f &position, const Vector2f &direction);
  void update(float deltaTime);
  Sprite &getSprite() { return laserSprite; }

  void setPosition(const Vector2f &pos) { laserSprite.setPosition(pos); }
  bool isOutsideWindow(const Vector2u &windowSize) const;

private:
  Texture *p_laserTexture;
  Sprite laserSprite;
  Vector2f direction;
  static constexpr float speed = 300.f;
};