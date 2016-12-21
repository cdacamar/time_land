#pragma once

#include <chrono>
#include <vector>

#include <SFML/Graphics.hpp>

#include <stlab/future.hpp>

#include <entities/environment.h>
#include <util/shared_entity.h>

namespace game_states {

namespace detail {

struct game {
  std::vector<env::environment_t> worlds;

  game(int width, int height);
};

const env::environment_t& current(const game& g);
      env::environment_t& current(game& g);
void                      commit(game& g);
void                      undo(game& g);

} // namespace detail

using game_t = detail::game;

// game ops
stlab::future<void> draw(const game_t& game, sf::RenderWindow& window);
stlab::future<void> process_event(game_t& game, const sf::Event& e, sf::RenderWindow& window);


} // namespace game_states
