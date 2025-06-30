#pragma once
#include "asteroid.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

namespace helper {

// Based on the project structure suggested in SFML Game Development By Example
// (2016), written by Raimondas Pupius, page 20.
class Window {
private:
  void Setup(const std::string &title, const sf::Vector2u &size);
//  void Destroy();
  void Create();

  sf::RenderWindow m_window;
  sf::Vector2u m_windowSize;
  std::string m_windowTitle;
  bool m_isDone;
  bool m_isFullscreen;

public:
  Window(const std::string &title, const sf::Vector2u &size);
  ~Window();
  
  void Destroy();

    
  void BeginDraw();
  void EndDraw();

  void Update();

  bool IsDone();
  bool IsFullscreen();
  sf::Vector2u GetWindowSize();

  /* void ToggleFullscreen(); */

  void Draw(sf::Drawable &obj);
  void RenderAsteroids(std::vector<Asteroid> &vec);
    
  sf::RenderWindow& GetWindow() { return m_window; }
};

} // namespace helper