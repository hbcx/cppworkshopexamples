#include <iostream>
#include <span>
#include <vector>

int main() {
    // Anti-pattern: holding a span into a vector across an operation that
    // reallocates the vector's buffer. The span keeps pointing at the old,
    // freed block.
    std::vector<int> v{1, 2, 3};
    std::span<int> s = v;
    const int* before = s.data(); // where the span (and the vector) point now

    // Grow the vector so its storage is certainly moved to a new block.
    v.reserve(v.capacity() + 100);
    const int* after = v.data();

    // We do NOT read through s -- that would be undefined. We show that the
    // storage moved, so the old span no longer refers to the vector's data.
    std::cout << "vector storage moved: " << (before != after) << '\n';
    std::cout << "old span still points at the old block (now stale)\n";

    // RIGHT: make the span AFTER the vector is done changing size.
    std::span<int> fresh = v;
    std::cout << "fresh span size: " << fresh.size() << ", back: " << fresh.back() << '\n';
    return 0;
}
