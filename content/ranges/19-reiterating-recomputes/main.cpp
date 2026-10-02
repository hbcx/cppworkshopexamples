#include <iostream>
#include <ranges>
#include <vector>

int calls = 0;

int main() {
    std::vector<int> v = {1, 2, 3, 4};
    auto expensive = v | std::views::transform([](int x){ ++calls; return x * x; });

    // BAD: iterating the same lazy view twice RECOMPUTES the transform each pass.
    calls = 0;
    for (int x : expensive) (void) x;   // 4 calls
    for (int x : expensive) (void) x;   // 4 more -- recomputed
    std::cout << "two passes over the view:    " << calls << " transform calls\n";   // 8

    // GOOD: materialise once, then reuse the container freely.
    calls = 0;
    std::vector<int> cached;
    for (int x : expensive) cached.push_back(x);   // 4 calls, once
    for (int x : cached) (void) x;                 // 0
    for (int x : cached) (void) x;                 // 0
    std::cout << "materialise then two passes: " << calls << " transform calls\n";   // 4
    return 0;
}
