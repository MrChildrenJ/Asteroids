#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>
#include <new>
#include <vector>
using namespace sf;
using namespace std;

class Asteroid : public sf::Drawable, public sf::Transformable {
public:
    Asteroid(Texture &texture, const Vector2f &startPosition,
             const Vector2f initialVelocity, float initialRotationSpeed);
    Asteroid(Texture &texture, const Vector2f &startPosition,
             const Vector2f initialVelocity, float initialRotationSpeed,
             bool small);
    //~Asteroid();
    void update(float deltaTime, const Vector2u &windowSize);
    FloatRect getBounds() const;
    const Sprite& getSprite() const { return sprite_asteroid; }
    
    static void initializeAsteroids(vector<Asteroid>& asteroids,
                                    const vector<unique_ptr<Texture>>& textures,
                                    const Vector2u& windowSize, int initialCount) {
        
        srand(static_cast<unsigned>(time(nullptr)));
        
        for (int i = 0; i < initialCount; ++i) {
            int texIndex = rand() % textures.size();

            float positionX, positionY, speedX {0.0}, speedY {0.0};
            // make asteroid randomly from left or right
            bool fromLeftOrRight = rand() % 2 == 0;
            
            if (fromLeftOrRight) {
                positionX = (rand() % 2 == 0) ? -50.f : windowSize.x + 50.f;
                positionY = static_cast<float>(rand() % windowSize.y);
                speedX = (positionX < 0) ? (static_cast<float>(rand() % 50 + 50)) : (-static_cast<float>(rand() % 50 + 50));
            } else {
                positionX = static_cast<float>(rand() % windowSize.x);
                positionY = (rand() % 2 == 0) ? -50.f : windowSize.y + 50.f;
                speedY = (positionY < 0) ? (static_cast<float>(rand() % 50 + 50)) : (-static_cast<float>(rand() % 50 + 50));
            }
            
            float rotationSpeed = static_cast<float>(rand() % 200 - 100);
            
            asteroids.emplace_back(*textures[texIndex], Vector2f(positionX, positionY), Vector2f(speedX, speedY), rotationSpeed);
        }
    }

    bool isHitByLaser(const Sprite& laserSprite) const;
    bool isSmall() const { return isSmallOne; }
    
    friend void breakApart(vector<Asteroid>& asteroids, size_t index, const vector<unique_ptr<Texture>>& textures);

private:
  Sprite sprite_asteroid;
  Vector2f velocity;
  float rotationSpeed;
  bool isSmallOne;

  virtual void draw(RenderTarget &target, RenderStates states) const override {
    states.transform *= getTransform();
    target.draw(sprite_asteroid, states);
  }
};

vector<unique_ptr<Texture>> loadTextures(const vector<string> &filePaths);
void updateAsteroids(vector<Asteroid> &asteroids, float deltaTime,
                     const Vector2u &windowSize);
void renderAsteroids(RenderWindow &window, const vector<Asteroid> &asteroids);