#include "ship.h"
#include "asteroid.h"
#include "laser.h"
#include "window.h"

Ship::Ship(float x, float y) : KeyDown(false), m_velocity(0) {
  this->setPosition(x, y);
  m_texture.loadFromFile("images/spaceship.png");
  sprite.setTexture(m_texture);
  sprite.setOrigin(sprite.getLocalBounds().width / 2.0f,
                   sprite.getLocalBounds().height / 2.0f);
  sprite.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f);

  float rotationRadians = getRotation() * M_PI / 180.f;
  m_direction = Vector2f(sin(rotationRadians), -cos(rotationRadians));
  cooldown.restart();
}

void Ship::handleInput(Event &e) {
  // Shoot laser
  if (e.type == Event::KeyPressed && e.key.code == Keyboard::Space)
    KeyDown.Space = true;
  else if (e.type == Event::KeyReleased && e.key.code == Keyboard::Space)
    KeyDown.Space = false;
  // Rotate left
  else if (e.type == Event::KeyPressed && e.key.code == Keyboard::Left)
    KeyDown.Left = true;
  else if (e.type == Event::KeyReleased && e.key.code == Keyboard::Left)
    KeyDown.Left = false;
  // Rotate Right
  else if (e.type == Event::KeyPressed && e.key.code == Keyboard::Right)
    KeyDown.Right = true;
  else if (e.type == Event::KeyReleased && e.key.code == Keyboard::Right)
    KeyDown.Right = false;
  // Move forward
  else if (e.type == Event::KeyPressed && e.key.code == Keyboard::Up)
    KeyDown.Up = true;
  else if (e.type == Event::KeyReleased && e.key.code == Keyboard::Up)
    KeyDown.Up = false;
}

void Ship::update(vector<Laser> &lasers, float &deltaTime) {
  float rotationRadians = sprite.getRotation() * M_PI / 180.f;
  m_direction = sf::Vector2f(sin(rotationRadians), -cos(rotationRadians));

  if (m_velocity > 0.f)
    m_velocity -= deltaTime;
  else
    m_velocity = 0.f;

  if (KeyDown.Space &&
      cooldown.getElapsedTime().asMilliseconds() >= RATE_OF_FIRE) {
    Laser newLaser(getNosePosition(), m_direction);
    lasers.push_back(newLaser);
    cooldown.restart();
  }

  if (KeyDown.Left)
    sprite.rotate(-TURN_SPEED);
  if (KeyDown.Right)
    sprite.rotate(TURN_SPEED);

  if (KeyDown.Up) {
    m_velocity = ACCELERATION * deltaTime;
    sprite.move(sin(sprite.getRotation() / 360 * 2 * M_PI) * m_velocity,
                -cos(sprite.getRotation() / 360 * 2 * M_PI) * m_velocity);

    Vector2f pos = sprite.getPosition();
    float halfWidth = sprite.getGlobalBounds().width / 2.f;
    float halfHeight = sprite.getGlobalBounds().height / 2.f;

    // Check for left boundary
    if (pos.x - halfWidth < 0) {
      sprite.setPosition(halfWidth, pos.y); // Set to left edge
    }
    // Check for right boundary
    else if (pos.x + halfWidth > WINDOW_WIDTH) {
      sprite.setPosition(WINDOW_WIDTH - halfWidth, pos.y); // Set to right edge
    }
    // Check for top boundary
    if (pos.y - halfHeight < 0) {
      sprite.setPosition(pos.x, halfHeight); // Set to top edge
    }
    // Check for bottom boundary
    else if (pos.y + halfHeight > WINDOW_HEIGHT) {
      sprite.setPosition(pos.x,
                         WINDOW_HEIGHT - halfHeight); // Set to bottom edge
    }
  } else {
    // TODO maintain intertia and slow down when key released
  }
}

Vector2f Ship::getNosePosition() const {
  float rotationRadians = sprite.getRotation() * M_PI / 180.f;
  Vector2f direction(sin(rotationRadians),
                     -cos(rotationRadians)); // SFML uses inverted Y

  float noseLength = sprite.getGlobalBounds().height / 2.f; // Adjust as needed
  return sprite.getPosition() + direction * noseLength;
}

bool Ship::checkCollision(const Asteroid &asteroid) {
  return getGlobalBounds().intersects(asteroid.getBounds());
}

sf::FloatRect Ship::getGlobalBounds() const { return sprite.getGlobalBounds(); }