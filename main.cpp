#include <random>

#include <SFML/Graphics.hpp>

#include <stlab/future.hpp>

int main() {
  sf::RenderWindow window;
  window.create(sf::VideoMode{800, 600}, "simple window");
  window.setFramerateLimit(120);

  std::mt19937 gen{std::random_device{}()};
  std::uniform_int_distribution<> dis{0, 3};
  sf::Color colors[] = {
    sf::Color::Blue,
    sf::Color::Black,
    sf::Color::Green,
    sf::Color::White
  };

  while (window.isOpen()) {
    sf::Event e;
    while (window.pollEvent(e)) {
      if (e.type == sf::Event::Closed ||
         (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape)) {
        window.close();
      }
    }

    auto task = stlab::async(stlab::default_scheduler{}, [&colors, &gen, &dis] () -> sf::Color { return colors[dis(gen)]; });
    while (!task.get_try());
    window.clear(*task.get_try());
    window.display();
  }
}
