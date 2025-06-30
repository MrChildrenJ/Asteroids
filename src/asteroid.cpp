#include "asteroid.h"

Asteroid::Asteroid(Texture& texture, const Vector2f& startPosition, const Vector2f initialVelocity, float initialRotationSpeed) {
    this->velocity = initialVelocity;
    this->rotationSpeed = initialRotationSpeed;
    this->isSmallOne = false;
    sprite_asteroid.setTexture(texture);
    
    if (!isSmallOne)
        sprite_asteroid.setScale(50.f / texture.getSize().x, 50.f / texture.getSize().y);
    else
        sprite_asteroid.setScale(25.f / texture.getSize().x, 25.f / texture.getSize().y);
    
    setPosition(startPosition);
}

Asteroid::Asteroid(Texture& texture, const Vector2f& startPosition, const Vector2f initialVelocity, float initialRotationSpeed, bool small) {
    this->velocity = initialVelocity;
    this->rotationSpeed = initialRotationSpeed;
    this->isSmallOne = small;
    sprite_asteroid.setTexture(texture);
    
    if (!isSmallOne)
        sprite_asteroid.setScale(50.f / texture.getSize().x, 50.f / texture.getSize().y);
    else
        sprite_asteroid.setScale(25.f / texture.getSize().x, 25.f / texture.getSize().y);
    
    setPosition(startPosition);
}

void Asteroid::update(float deltaTime, const Vector2u& windowSize) {
    move(velocity * deltaTime);
    rotate(rotationSpeed * deltaTime);
    
    Vector2f pos = getPosition();
    if (pos.x < 0)  pos.x = windowSize.x;
    if (pos.x > windowSize.x)   pos.x = 0;
    if (pos.y < 0)  pos.y = windowSize.y;
    if (pos.y > windowSize.y)   pos.y = 0;
    setPosition(pos);
}

FloatRect Asteroid::getBounds() const {
    return getTransform().transformRect(sprite_asteroid.getGlobalBounds());
}

bool Asteroid::isHitByLaser(const Sprite& laserSprite) const {
    return getBounds().intersects(laserSprite.getGlobalBounds());
}

void breakApart(vector<Asteroid>& asteroids, size_t index,
                const vector<unique_ptr<Texture>>& textures) {
    
    Asteroid& aster = asteroids[index];
    
    if (aster.isSmallOne) {     
        asteroids.erase(asteroids.begin() + index);
        return;
    }
    
    int texIndex1 = rand() % textures.size();
    int texIndex2 = rand() % textures.size();
    
    float originalScale = aster.sprite_asteroid.getScale().x;
    Vector2f originalPosition = aster.getPosition();
    float originalRotationSpeed = aster.rotationSpeed;
    
    // create two smaller asteroids with half the size
    Asteroid smallAsteroid1(*textures[texIndex1], originalPosition,
                            Vector2f(rand() % 200 - 100, rand() % 200 - 100),
                            originalRotationSpeed / 2, true);
    
    Asteroid smallAsteroid2(*textures[texIndex2], originalPosition,
                            Vector2f(rand() % 200 - 100, rand() % 200 - 100),
                            originalRotationSpeed / 2, true);
    
    // Replace the original asteroid with the new smaller asteroids
    asteroids[index] = smallAsteroid1;
    asteroids.insert(asteroids.begin() + index + 1, smallAsteroid2);
}

vector<unique_ptr<Texture> > loadTextures(const vector<string>& filePaths) {
    vector<unique_ptr<Texture> > textures;
    for (const auto& path : filePaths) {
        auto tex = make_unique<Texture>();
        tex->loadFromFile(path);
        textures.push_back(move(tex));
    }
    return textures;
}

void updateAsteroids(vector<Asteroid>& asteroids, float deltaTime, const Vector2u& windowSize) {
    for (auto& aster : asteroids)
        aster.update(deltaTime, windowSize);
}

void renderAsteroids(RenderWindow& window, const vector<Asteroid>& asteroids) {
    for (const auto& aster : asteroids)
        window.draw(aster);
}
