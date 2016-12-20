#pragma once

#include <chrono>

#include <SFML/Graphics.hpp>

class game {
public:
  game() = default;

  void update(std::chrono::milliseconds dt);
  void draw(sf::RenderWindow& window);
  void process_event(const sf::Event& e);
};
