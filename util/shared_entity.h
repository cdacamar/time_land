#pragma once

#include <memory>
#include <utility>

template <typename T>
class shared_entity {
  std::shared_ptr<const T> self_;
public:
  shared_entity(T e): self_{std::make_shared<T>(std::move(e))} { }

  template <typename... Args>
  shared_entity(Args&&... args): self_{std::make_shared<T>(std::forward<Args>(args)...)} { }

  const T& get() const { return *self_; }
};
