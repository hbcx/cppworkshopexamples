#ifndef PIMPL_COPY_CONFIG_HPP
#define PIMPL_COPY_CONFIG_HPP

#include <memory>
#include <string>

// A pimpl class made into a copyable VALUE: the copy operations deep-copy Impl.
class Config {
public:
    Config();
    Config(const Config& other);            // deep copy
    Config& operator=(const Config& other); // deep copy
    ~Config();

    void set(const std::string& key, const std::string& value);
    std::string get(const std::string& key) const;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

#endif
