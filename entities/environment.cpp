#include <entities/environment.h>

#include <cstdlib>

namespace env {

// TODO: find a way to clean this up. This is pretty ugly right now
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
      img.create(tile_size, tile_size, sf::Color::Red);
      // paint ugly color
      for (int tx = 0;tx != tile_size;++tx) {
        for (int ty = 0;ty != tile_size;++ty) {
          if (ty + y * tile_size > hmap[tx + x * tile_size]) {
            img.setPixel(tx, ty, sf::Color::Green);
          }
        }
      }
      image_row.emplace_back(std::move(img));
      tex.loadFromImage(image_row.back().get());
      texture_row.emplace_back(std::move(tex));

      spr.setTexture(texture_row.back().get());
      spr.setPosition(static_cast<float>(x * tile_size), static_cast<float>(y * tile_size));

      sprite_row.emplace_back(std::move(spr));
    }

    sprite_lst.emplace_back(std::move(sprite_row));
    image_lst.emplace_back(std::move(image_row));
    texture_lst.emplace_back(std::move(texture_row));
  }

  // address remainders
  if (rw.rem) {
    for (int y = 0;y != size_h;++y) {
      img.create(rw.rem, tile_size, sf::Color::Red);
      // paint
      for (int tx = 0;tx != rw.rem;++tx) {
        for (int ty = 0;ty != tile_size;++ty) {
          if (ty + y * tile_size > hmap[tx + tile_size * size_w]) {
            img.setPixel(tx, ty, sf::Color::Green);
          }
        }
      }

      auto img_l = image_lst[y].get();
      img_l.emplace_back(std::move(img));
      image_lst[y] = img_l;

      tex.loadFromImage(image_lst[y].get().back().get());
      auto tex_l = texture_lst[y].get();
      tex_l.emplace_back(std::move(tex));
      texture_lst[y] = tex_l;

      spr.setTexture(texture_lst[y].get().back().get());
      spr.setPosition(static_cast<float>(tile_size * size_w), static_cast<float>(y * tile_size));
      auto spr_l = sprite_lst[y].get();
      spr_l.emplace_back(std::move(spr));
      sprite_lst[y] = spr_l;
    }
  }
  if (rh.rem) {
    std::vector<detail::sprite_t>  sprite_row;
    std::vector<detail::image_t>   image_row;
    std::vector<detail::texture_t> texture_row;

    sprite_row.reserve(size_w);
    image_row.reserve(size_w);
    texture_row.reserve(size_w);
    for (int x = 0;x != size_w;++x) {
      img.create(rw.rem, tile_size, sf::Color::Red);
      // paint
      for (int tx = 0;tx != tile_size;++tx) {
        for (int ty = 0;ty != rh.rem;++ty) {
          if (ty + size_h * tile_size > hmap[tx + tile_size * size_w]) {
            img.setPixel(tx, ty, sf::Color::Green);
          }
        }
      }
      image_row.emplace_back(std::move(img));
      tex.loadFromImage(image_row.back().get());
      texture_row.emplace_back(std::move(tex));

      spr.setTexture(texture_row.back().get());
      spr.setPosition(static_cast<float>(x * tile_size), static_cast<float>(size_h * tile_size));

      sprite_row.emplace_back(std::move(spr));
    }

    if (rw.rem) { // special case, partial tile-x within our last row
      img.create(rw.rem, rh.rem, sf::Color::Red);
      // paint
      for (int tx = 0;tx != rw.rem;++tx) {
        for (int ty = 0;ty != rh.rem;++ty) {
          if (ty + size_h * tile_size > hmap[tx + tile_size * size_w]) {
            img.setPixel(tx, ty, sf::Color::Green);
          }
        }
      }
      image_row.emplace_back(std::move(img));
      tex.loadFromImage(image_row.back().get());
      texture_row.emplace_back(std::move(tex));

      spr.setTexture(texture_row.back().get());
      spr.setPosition(static_cast<float>(size_w * tile_size), static_cast<float>(size_h * tile_size));

      sprite_row.emplace_back(std::move(spr));
    }
    image_lst.emplace_back(std::move(image_row));
    texture_lst.emplace_back(std::move(texture_row));
    sprite_lst.emplace_back(std::move(sprite_row));
  }

  return detail::environment{std::move(hmap),
                             std::move(sprite_lst),
                             std::move(image_lst),
                             std::move(texture_lst)};
}

} // namespace env
