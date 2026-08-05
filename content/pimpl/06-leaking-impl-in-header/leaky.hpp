#ifndef PIMPL_LEAK_LEAKY_HPP
#define PIMPL_LEAK_LEAKY_HPP

#include <cstddef>
#include <string>
#include <vector> // LEAK: every client of this header now includes <vector>

// The private member sits in the header, so the layout is exposed and any
// change to it recompiles all clients and breaks the ABI.
class LeakyCache {
public:
    void put(const std::string& value);
    std::size_t size() const;

private:
    std::vector<std::string> items_; // LEAK: internals visible in the header
};

#endif
