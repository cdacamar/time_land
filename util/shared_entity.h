#pragma once

#include <memory>
#include <utility>

namespace util {

template <typename T>
class shared_entity {
  std::shared_ptr<const T> self_;
public:
  shared_entity(const shared_entity&) = default;
  shared_entity(shared_entity&&)      = default;

  shared_entity(T e): self_{std::make_shared<T>(std::move(e))} { }

  shared_entity& operator=(const shared_entity&) = default;
  shared_entity& operator=(shared_entity&&)      = default;

  const T& get() const { return *self_; }
};

} // namespace util
