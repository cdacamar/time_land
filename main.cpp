#include <random>

#include <SFML/Graphics.hpp>

#include <stlab/future.hpp>

#include <entities/environment.h>

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
  (void)colors;

  auto world = env::make_environment(600, 800);
  std::vector<stlab::future<void>> sprite_tasks(world.get().sprite_tiles.get().size()); // we want a future for each row

  while (window.isOpen()) {
    sf::Event e;
    while (window.pollEvent(e)) {
      if (e.type == sf::Event::Closed ||
         (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape)) {
        window.close();
      }
    }
    window.clear(sf::Color::White);

    // not 100% sure sf::RenderWindow::draw() is thread safe but I'm doing it anyway...
    // YOLO
    for (std::size_t i = 0, last = world.get().sprite_tiles.get().size();
        i != last;++i) {
      sprite_tasks.at(i) = stlab::async(stlab::default_scheduler{},
          [&window, row = world.get().sprite_tiles.get()[i]] {
            for (auto& spr : row.get()) {
              window.draw(spr.get());
            }
          });
    }
    auto task = stlab::when_all(stlab::default_scheduler{},
        [&window] { window.display(); },
        std::make_pair(std::begin(sprite_tasks), std::end(sprite_tasks)));
    while (!task.get_try());
  }
}
