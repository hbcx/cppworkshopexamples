#include <iostream>
#include <ranges>
#include <vector>

int main() {
    std::vector<int> v = {1, 2, 3};

    // A view does NOT snapshot its source -- it refers to it lazily. So changing
    // the source AFTER building the view changes what the view yields.
    auto doubled = v | std::views::transform([](int x){ return x * 2; });

    std::cout << "before change:";
    for (int x : doubled) std::cout << " " << x;   // 2 4 6
    std::cout << "\n";

    v.push_back(4);   // change the SOURCE after the view was built
    v[0] = 100;

    std::cout << "after change: ";
    for (int x : doubled) std::cout << " " << x;   // 200 4 6 8 -- the view reflects it
    std::cout << "\n";

    // If you need a STABLE snapshot, materialise into a container. Now later
    // changes to the source cannot touch it.
    std::vector<int> snapshot;
    for (int x : doubled) snapshot.push_back(x);   // {200, 4, 6, 8}
    v[0] = 1;                                       // change the source again
    std::cout << "snapshot stays:";
    for (int x : snapshot) std::cout << " " << x;   // 200 4 6 8 -- unaffected
    std::cout << "\n";
    return 0;
}
