#include <generator>
#include <iostream>
#include <ranges>

// Counts every co_yield that actually executes, so we can see the forwarding cost
// of nesting generators the wrong way.
long g_yields = 0;

// ANTI-PATTERN: nest generators by re-yielding each element by hand. Every level
// of nesting runs its own co_yield for every element, so an element from the
// bottom is yielded once per level -- work proportional to the depth per element.
std::generator<int> manualChain(int depth) {
    if (depth == 0) {
        for (int i = 0; i < 5; ++i) {
            ++g_yields;
            co_yield i;
        }
    } else {
        for (int x : manualChain(depth - 1)) {
            ++g_yields;
            co_yield x;                     // re-yield: one extra co_yield per level
        }
    }
}

// FIX: splice the inner generator in with elements_of. The elements are not
// re-yielded at each level, so the cost does not grow with the depth.
std::generator<int> elementsChain(int depth) {
    if (depth == 0) {
        for (int i = 0; i < 5; ++i) {
            ++g_yields;
            co_yield i;
        }
    } else {
        co_yield std::ranges::elements_of(elementsChain(depth - 1));
    }
}

int main() {
    const int depth = 4;   // 5 leaf elements, nested 4 levels deep

    g_yields = 0;
    std::cout << "manual re-yield:";
    for (int v : manualChain(depth))
        std::cout << " " << v;
    std::cout << "  -> " << g_yields << " co_yields for 5 elements\n";

    g_yields = 0;
    std::cout << "elements_of:    ";
    for (int v : elementsChain(depth))
        std::cout << " " << v;
    std::cout << "  -> " << g_yields << " co_yields for 5 elements\n";
    return 0;
}
