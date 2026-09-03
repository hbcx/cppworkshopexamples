#include <functional>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

// A stand-in for an expensive-to-copy object; count copies to prove cref avoids
// them. A move constructor is defaulted so only real copies are counted.
struct BigConfig {
    std::string data;
    static int copies;
    explicit BigConfig(std::string d) : data(std::move(d)) {}
    BigConfig(const BigConfig& o) : data(o.data) { ++copies; }
    BigConfig(BigConfig&&) noexcept = default;
};
int BigConfig::copies = 0;

void report(const BigConfig& c) { std::cout << "config: " << c.data << "\n"; }

int main() {
    BigConfig cfg{"host=prod;retries=3"};

    // std::cref makes a std::reference_wrapper<const BigConfig>: a read-only
    // reference you can store and pass by value with NO copy of the big object.
    auto handle = std::cref(cfg);
    report(handle);   // implicit conversion to const BigConfig&
    std::cout << "copies so far: " << BigConfig::copies << "\n";   // 0

    // Build a lookup table of const views over objects owned elsewhere -- again
    // no copies, and the table cannot modify them.
    std::vector<std::reference_wrapper<const BigConfig>> views{std::cref(cfg)};
    std::cout << "view[0]: " << views[0].get().data << "\n";
    std::cout << "total copies: " << BigConfig::copies << "\n";     // still 0
    return 0;
}
