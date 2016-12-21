#include <chrono>
#include <iostream>

#include <SFML/Graphics.hpp>

#include <stlab/future.hpp>

#include <entities/environment.h>
#include <game_states/game.h>

class stopwatch {
  using clock_t = std::chrono::high_resolution_clock;

  clock_t::time_point start_;
  clock_t::time_point stop_;

public:
  stopwatch(): start_{clock_t::now()}, stop_{start_} { }

  void start() { start_ = clock_t::now(); }
  void stop()  { stop_  = clock_t::now(); }

  clock_t::duration ticks() { return stop_ - start_; }
};

int main() {
  stopwatch sw;
  sf::RenderWindow window;
  window.create(sf::VideoMode{800, 600}, "simple window");
  window.setFramerateLimit(120);

  game_states::game_t main_game{800, 600};

  double        average_render_time = 0.0;
  std::uint64_t render_count        = 0;

  while (window.isOpen()) {
    sf::Event e;
    while (window.pollEvent(e)) {
      if (e.type == sf::Event::Closed ||
         (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape)) {
        window.close();
      }
      else {
        auto poll_task = game_states::process_event(main_game, e, window);
        while (!poll_task.get_try());
      }
    }
    window.clear(sf::Color::White);

    // not 100% sure sf::RenderWindow::draw() is thread safe but I'm doing it anyway...
    // YOLO
    sw.start();
    auto draw_game_task = game_states::draw(main_game, window);
    while(!draw_game_task.get_try());
    window.display();
    sw.stop();

    average_render_time = (average_render_time * render_count + std::chrono::duration_cast<std::chrono::milliseconds>(sw.ticks()).count()) / (render_count + 1);
    ++render_count;
    std::cout<<"current average: "<<average_render_time<<'\r';
  }
}
