#include "clean.hpp"

#include <vector> // the heavy include lives HERE, not in the header

struct CleanCache::Impl {
    std::vector<std::string> items;
};

CleanCache::CleanCache() : impl_(std::make_unique<Impl>()) {}
CleanCache::~CleanCache() = default;

void CleanCache::put(const std::string& value) { impl_->items.push_back(value); }

std::size_t CleanCache::size() const { return impl_->items.size(); }
