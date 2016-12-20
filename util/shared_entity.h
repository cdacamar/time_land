#pragma once

#include <memory>

template <typename T>
class shared_entity {
  std::shared_ptr<const T> self_;
public:
  shared_entity(T e): self_{std::make_shared<T>(std::move(s))} { }

  const T& get() const { return *self_; }
};
