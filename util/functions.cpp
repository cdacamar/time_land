#include <cmath>

#include <unordered_map>

namespace util {

double cached_sqrt(int n) {
  thread_local static std::unordered_map<int, double> cache;

  auto val = cache.find(n);
  if (val != std::end(cache)) {
    return val->second;
  }

  auto r = std::sqrt(n);
  cache[n] = r;
  return r;
}

} // namespace util
