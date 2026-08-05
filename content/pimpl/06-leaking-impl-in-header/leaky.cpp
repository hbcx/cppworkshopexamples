#include "leaky.hpp"

void LeakyCache::put(const std::string& value) { items_.push_back(value); }

std::size_t LeakyCache::size() const { return items_.size(); }
