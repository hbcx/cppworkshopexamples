#include <ranges>
#include <vector>
#include <iostream>

// ranges::to is EAGER: it builds a real container right there. Put it in the
// MIDDLE of a pipeline and you throw laziness away -- you allocate a full
// intermediate container and process all of it, instead of letting elements flow
// through one at a time. to belongs at the END, where you actually need a container.

// A transform that counts its calls, so the wasted work is visible.
static int calls = 0;
int expensive(int x) { ++calls; return x * x; }

int main() {
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8};

    // BAD: materialize everything, THEN take three. The transform ran for all
    // eight elements even though only three results are used.
    calls = 0;
    auto all = v | std::views::transform(expensive)
                 | std::ranges::to<std::vector>();     // eager, too early
    auto firstThree = all | std::views::take(3) | std::ranges::to<std::vector>();
    std::cout << "to in the middle: transform ran " << calls
              << " times for " << firstThree.size() << " results\n";

    // GOOD: stay lazy and materialize only at the end. take(3) limits the work
    // before the transform is forced, so it runs just three times.
    calls = 0;
    auto lazyFirstThree = v | std::views::transform(expensive)
                            | std::views::take(3)
                            | std::ranges::to<std::vector>();  // to only at the end
    std::cout << "to at the end:    transform ran " << calls
              << " times for " << lazyFirstThree.size() << " results\n";
    return 0;
}
