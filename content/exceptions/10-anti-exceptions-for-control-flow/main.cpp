// Anti-pattern: using exceptions for ordinary control flow -- here, signaling
// "not found" by throwing. Exceptions are for the exceptional: a not-found is a
// normal, expected outcome of a search, so it belongs in the return type. Throw
// on the rare error path, not the common one; a value (or std::optional) is
// clearer and far cheaper on the hot path.
#include <iostream>
#include <vector>
#include <stdexcept>
#include <optional>
#include <cstddef>

const std::vector<int> data = {4, 8, 15, 16, 23, 42};

// WRONG: a miss is expected, yet it throws -- the caller must wrap every lookup
// in try/catch, and each miss pays the cost of an exception.
int find_wrong(int value) {
    for (std::size_t i = 0; i < data.size(); ++i) {
        if (data[i] == value) return static_cast<int>(i);
    }
    throw std::runtime_error("not found");
}

// RIGHT: a miss is a normal result, so return std::optional -- an empty result
// means "not here". No try/catch, no exception cost on a routine lookup.
std::optional<int> find_right(int value) {
    for (std::size_t i = 0; i < data.size(); ++i) {
        if (data[i] == value) return static_cast<int>(i);
    }
    return std::nullopt;
}

int main() {
    // The throwing version forces a try/catch around a routine miss.
    try {
        std::cout << "wrong found 15 at: " << find_wrong(15) << '\n';
        std::cout << "wrong find 99: " << find_wrong(99) << '\n';    // throws
    }
    catch (const std::exception& e) {
        std::cout << "wrong threw for 99: " << e.what() << '\n';
    }

    // The optional version handles a miss as a plain value.
    if (auto pos = find_right(15)) std::cout << "right found 15 at: " << *pos << '\n';
    if (!find_right(99))           std::cout << "right: 99 not found\n";
    return 0;
}
