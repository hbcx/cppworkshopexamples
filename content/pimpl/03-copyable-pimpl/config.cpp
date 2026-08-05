#include "config.hpp"

#include <map>

struct Config::Impl {
    std::map<std::string, std::string> values;
};

Config::Config() : impl_(std::make_unique<Impl>()) {}

// Deep copy: allocate a fresh Impl initialised from the source's Impl.
Config::Config(const Config& other) : impl_(std::make_unique<Impl>(*other.impl_)) {}

// Deep copy: copy the source Impl's contents into our own.
Config& Config::operator=(const Config& other) {
    if (this != &other) *impl_ = *other.impl_;
    return *this;
}

Config::~Config() = default;

void Config::set(const std::string& key, const std::string& value) {
    impl_->values[key] = value;
}

std::string Config::get(const std::string& key) const {
    std::map<std::string, std::string>::const_iterator it = impl_->values.find(key);
    return it == impl_->values.end() ? "" : it->second;
}
