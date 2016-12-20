#include <entities/environment.h>

#include <cstdlib>

namespace env {

environment_t make_environment(int h, int w) {
  // we need to partition the height and width such that we get
  // some number of tiles to cover the entire width and height
  const int tile_size = detail::environment::tile_size;
  auto rw = std::div(w, tile_size); // start with 64x64 sized tiles
  auto rh = std::div(h, tile_size);

  // for now, let's not worry about the case where a 64x64 tile does not fit within the window
  auto size_h = rh.quot;
  auto size_w = rw.quot;

  std::vector<detail::shared_list_t<detail::sprite_t>>  sprite_lst;
  std::vector<detail::shared_list_t<detail::image_t>>   image_lst;
  std::vector<detail::shared_list_t<detail::texture_t>> texture_lst;
  sprite_lst.reserve(size_h);
  image_lst.reserve(size_h);
  texture_lst.reserve(size_h);

  sf::Sprite  spr;
  sf::Image   img;
  sf::Texture tex;

  // height map
  std::vector<int> hmap(w);
  auto cal_height = h / 2;
  for (auto& ht : hmap) ht = cal_height;

  for (int y = 0;y != size_h;++y) {
    std::vector<detail::sprite_t>  sprite_row;
    std::vector<detail::image_t>   image_row;
    std::vector<detail::texture_t> texture_row;

    sprite_row.reserve(size_w);
    image_row.reserve(size_w);
    texture_row.reserve(size_w);
    for (int x = 0;x != size_w;++x) {
      // logic for painting tiles
      img.create(tile_size, tile_size, sf::Color::Transparent);
      // paint ugly color
      for (int tx = 0;tx != tile_size;++tx) {
        for (int ty = 0;ty != tile_size;++ty) {
          if (ty + y * tile_size > hmap[tx + x * tile_size]) {
            img.setPixel(tx, ty, sf::Color::Green);
          }
        }
      }
      tex.loadFromImage(img);

      spr.setTexture(tex);
      spr.setPosition(static_cast<float>(x * tile_size), static_cast<float>(y * tile_size));

      sprite_row.emplace_back(std::move(spr));
      image_row.emplace_back(std::move(img));
      texture_row.emplace_back(std::move(tex));
    }

    sprite_lst.emplace_back(std::move(sprite_row));
    image_lst.emplace_back(std::move(image_row));
    texture_lst.emplace_back(std::move(texture_row));
  }

  using sp_t = shared_entity<
                 std::vector<
                   shared_entity<
                     std::vector<shared_entity<detail::sprite_t>>
                   >
                 >
               >;
  sp_t s = sprite_lst;

  detail::sprite_list_t sl = sprite_lst;
  detail::image_list_t il = std::move(image_lst);
  detail::texture_list_t tl = std::move(texture_lst);
  detail::environment e{hmap, sprite_lst, image_lst, texture_lst};
  return detail::environment{std::move(hmap),
                             std::move(sprite_lst),
                             std::move(image_lst),
                             std::move(texture_lst)};
}
