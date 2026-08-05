#ifndef PIMPL_LEAK_CLEAN_HPP
#define PIMPL_LEAK_CLEAN_HPP

#include <cstddef>
#include <memory>
#include <string>

// No <vector> here: internals are hidden behind a forward-declared Impl.
class CleanCache {
public:
    CleanCache();
    ~CleanCache();
    void put(const std::string& value);
    std::size_t size() const;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

#endif
