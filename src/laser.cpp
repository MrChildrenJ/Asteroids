#include "laser.h"

Laser::Laser(const Vector2f &position, const Vector2f &direction) {
    this->direction = direction;
    
    p_laserTexture = new sf::Texture();
    (*p_laserTexture).loadFromFile("images/laser.png");
    laserSprite.setTexture(*p_laserTexture);
    laserSprite.setPosition(position);
    laserSprite.setScale(1.f, 1.f);
    
    float angle = atan2(direction.y, direction.x) * 180.f / M_PI + 90;
    laserSprite.setRotation(angle);
}

bool Laser::isOutsideWindow(const Vector2u &windowSize) const {
  FloatRect bounds = laserSprite.getGlobalBounds();
  return bounds.top + bounds.height < 0 || bounds.top > windowSize.y ||
         bounds.left + bounds.width < 0 || bounds.left > windowSize.x;
}

void Laser::update(float deltaTime) {
  laserSprite.move(direction * speed * deltaTime);
}