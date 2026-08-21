#include <algorithm>
#include <cstddef>
#include <iostream>
#include <span>
#include <vector>

int main() {
    // Anti-pattern: assuming span::operator[] or subspan is bounds-checked. They
    // are NOT: an out-of-range index or a bad subspan offset is undefined
    // behaviour, and span has no .at() before C++26.
    std::vector<int> v{1, 2, 3};
    std::span<int> s = v;

    // WRONG (undefined -- shown as comments, never run):
    //   int bad = s[5];              // index past the end -> UB
    //   auto sub = s.subspan(10);    // offset > size() -> UB

    // RIGHT: the size travels with the span, so check against it.
    std::size_t i = 5;
    if (i < s.size())
        std::cout << "s[" << i << "] = " << s[i] << '\n';
    else
        std::cout << "index " << i << " is out of range (size " << s.size() << ")\n";

    // RIGHT: clamp a subspan offset so the subview is always valid.
    std::size_t offset = 10;
    std::size_t safe = std::min(offset, s.size());
    std::span<int> tail = s.subspan(safe);
    std::cout << "clamped tail size: " << tail.size() << '\n';
    return 0;
}
