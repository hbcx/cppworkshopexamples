#include <expected>
#include <iostream>
#include <optional>
#include <variant>   // std::monostate

// Anti-pattern: expected<T, E> where E carries no information -- here
// std::monostate, a placeholder "error" with nothing to say. The error channel
// only duplicates what has_value() already tells you.
std::expected<int, std::monostate> findExpected(bool hit) {
    if (hit) return 42;
    return std::unexpected(std::monostate{});   // "failed", but why? nothing here
}

// Fix: when absence has no reason to report, optional<T> says exactly that and
// nothing more -- one empty state, no dead error type to construct and ignore.
std::optional<int> findOptional(bool hit) {
    if (hit) return 42;
    return std::nullopt;
}

int main() {
    std::cout << std::boolalpha;

    auto e = findExpected(false);
    std::cout << "expected: has_value=" << e.has_value()
              << " (error type carries no info)\n";

    auto o = findOptional(false);
    std::cout << "optional: has_value=" << o.has_value() << '\n';

    // Rule: expected when the failure has a reason worth carrying; optional when
    // absence is the whole story.
    std::cout << "optional hit: " << findOptional(true).value_or(-1) << '\n';
    return 0;
}
