#pragma once

#include <chrono>

#include <SFML/Graphics.hpp>

#include <stlab/future.hpp>

#include <entities/environment.h>
#include <util/shared_entity.h>

namespace game_states {

namespace detail {

struct game {
  env::environment_t world;

  game(int width, int height): world{env::make_environment(width, height)} { }

  void update(std::chrono::milliseconds dt);
  void process_event(const sf::Event& e);
};

} // namespace detail

using game_t = util::shared_entity<detail::game>;

// game ops
stlab::future<void> draw(game_t game, sf::RenderWindow& window);

} // namespace game_states
