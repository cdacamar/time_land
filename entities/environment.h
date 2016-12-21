#pragma once

#include <list>
#include <map>
#include <vector>

#include <SFML/Graphics.hpp>

#include <util/shared_entity.h>

namespace env {

namespace detail {

struct falling_column {
  int    top;
  int    bottom;
  double velocity;

  falling_column(): top{0}, bottom{0}, velocity{0.0} { }
};

// the layout of the environment is tile based
// this way we can change independent regions of
// the environment without having to copy the entire
// object

using sprite_t  = util::shared_entity<sf::Sprite>;
using image_t   = util::shared_entity<sf::Image>;
using texture_t = util::shared_entity<sf::Texture>;

template <typename T>
using shared_list_t = util::shared_entity<std::vector<T>>;

using sprite_list_t  = shared_list_t<shared_list_t<sprite_t>>;
using image_list_t   = shared_list_t<shared_list_t<image_t>>;
using texture_list_t = shared_list_t<shared_list_t<texture_t>>;

struct environment {
  static const int tile_size = 64;

  shared_list_t<int> pixel_hmap;
  sprite_list_t      sprite_tiles;
  image_list_t       image_tiles;
  texture_list_t     tex_tiles;
  const int          width;
  const int          height;
  std::map<int, std::list<falling_column>> collapsing_map;

  environment(int width, int height, shared_list_t<int> pixel_hmap,
              sprite_list_t sprite_tiles, image_list_t  image_tiles,
              texture_list_t tex_tiles):
    pixel_hmap{std::move(pixel_hmap)},
    sprite_tiles{std::move(sprite_tiles)},
    image_tiles{std::move(image_tiles)},
    tex_tiles{std::move(tex_tiles)},
    width{width},
    height{height} { }
};

} // namespace detail

using falling_column_t = detail::falling_column; // let's not make this shared quite yet
using environment_t    = util::shared_entity<detail::environment>;

// falling_column ops
falling_column_t merge(   const falling_column_t& lhs, const falling_column_t& rhs);
bool             intersects(const falling_column_t& lhs, const falling_column_t& rhs);


// environment ops
environment_t make_environment(int w, int h);
environment_t create_impact(environment_t e, const sf::CircleShape& blast);
sf::FloatRect bounding_rect(const environment_t& e);

} // namespace env
