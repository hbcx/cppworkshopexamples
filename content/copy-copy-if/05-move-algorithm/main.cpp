#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

static void print(const char* label, const std::vector<std::string>& v) {
    std::cout << label;
    for (const auto& s : v) std::cout << " [" << s << ']';
    std::cout << '\n';
}

int main() {
    std::vector<std::string> source{"alpha", "beta", "gamma"};
    std::vector<std::string> dest(3);

    // std::move (the algorithm) is std::copy that MOVES each element instead of
    // copying it. For expensive elements like strings this hands over the buffers
    // rather than duplicating them. The source elements are left in a valid but
    // UNSPECIFIED (moved-from) state, so we do not read their values afterwards --
    // only that the source still has the same number of (moved-from) slots.
    std::move(source.begin(), source.end(), dest.begin());

    print("dest after move:", dest);            // holds the values now
    std::cout << "source still has " << source.size()
              << " (moved-from) elements\n";

    // std::move_backward moves back to front, which is what you need to shift a
    // range toward higher positions within the SAME container without a value
    // overwriting one not yet moved. Here: shift the first three into the last
    // three slots of a six-element vector.
    std::vector<std::string> buf{"a", "b", "c", "", ""};
    std::move_backward(buf.begin(), buf.begin() + 3, buf.end());
    std::cout << "after move_backward, last three:";
    for (std::size_t i = 2; i < buf.size(); ++i) std::cout << " [" << buf[i] << ']';
    std::cout << '\n';
    return 0;
}
