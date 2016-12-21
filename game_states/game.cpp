#include <game_states/game.h>

#include <stlab/future.hpp>

namespace game_states {

namespace detail {

void game::update(std::chrono::milliseconds) {
}

void game::process_event(const sf::Event&) {
}

} // namespace detail

// game ops
stlab::future<void> draw(game_t game, sf::RenderWindow& window) {
  return stlab::async(
      stlab::default_scheduler{},
      [g = game, &window] {
        for (const auto& row : g.get().world.get().sprite_tiles.get()) {
          for (const auto& spr : row.get()) {
            window.draw(spr.get());
          }
        }
      });
}

} // namespace game_states
