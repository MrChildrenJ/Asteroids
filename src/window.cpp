#include "window.h"

helper::Window::Window(const std::string &title, const sf::Vector2u &size) {
  Setup(title, size);
}

helper::Window::~Window() { Destroy(); }

void helper::Window::Setup(const std::string &title, const sf::Vector2u &size) {
  m_windowTitle = title;
  m_windowSize = size;
  m_isFullscreen = false;
  m_isDone = false;
  Create();
}

void helper::Window::Create() {
  // auto style = (m_isFullscreen ? sf::Style::Fullscreen : sf::Style::Default);
  // Fixed window size with close button:
  auto style = sf::Style::Titlebar | sf::Style::Close;
  m_window.create({m_windowSize.x, m_windowSize.y, 32}, m_windowTitle, style);
}

void helper::Window::Destroy() { m_isDone = true; }

void helper::Window::BeginDraw() { m_window.clear(sf::Color::Black); }
void helper::Window::EndDraw() { m_window.display(); }

bool helper::Window::IsDone() { return m_isDone; }
bool helper::Window::IsFullscreen() { return m_isFullscreen; }

sf::Vector2u helper::Window::GetWindowSize() { return m_windowSize; }

void helper::Window::Draw(sf::Drawable &obj) { m_window.draw(obj); }
void helper::Window::RenderAsteroids(std::vector<Asteroid> &vec) {
  for (auto obj : vec) {
    m_window.draw(obj);
  }
}