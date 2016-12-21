#include <game_states/game.h>

#include <stlab/future.hpp>

namespace game_states {

namespace detail {

game::game(int width, int height) {
  worlds.emplace_back(env::make_environment(width, height));
}

const env::environment_t& current(const game& g) {
  return g.worlds.back();
}

env::environment_t& current(game& g) {
  return const_cast<env::environment_t&>(
          current(const_cast<const game&>(g))
    );
}

void commit(game& g) {
  g.worlds.emplace_back(current(g));
}

void undo(game& g) {
  if (g.worlds.size() > 1) g.worlds.pop_back();
}

} // namespace detail

// game ops
stlab::future<void> draw(const game_t& game, sf::RenderWindow& window) {
  return stlab::async(
      stlab::default_scheduler{},
      [&game, &window] {
        for (const auto& row : current(game).get().sprite_tiles.get()) {
          for (const auto& spr : row.get()) {
            window.draw(spr.get());
          }
        }
      });
}

stlab::future<void> process_event(game_t& game, const sf::Event& e, sf::RenderWindow& window) {
  return stlab::async(
      stlab::default_scheduler{},
      [&game, &e, &window] {
        if (e.type == sf::Event::MouseButtonReleased) {
          sf::CircleShape impact{30.f}; // 30px radius
          auto origin = window.mapPixelToCoords(sf::Mouse::getPosition(window));
          impact.setOrigin(origin);

          // commit the old current game state
          commit(game);
          current(game) = env::create_impact(current(game), impact);
        }
        else if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Z) {
          undo(game);
        }
      });
}

} // namespace game_states
