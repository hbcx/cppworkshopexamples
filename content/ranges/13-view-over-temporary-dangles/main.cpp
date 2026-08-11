#include <iostream>
#include <ranges>
#include <vector>

// GOOD: pipe an RVALUE container, so it is MOVED into an owning view that the
// returned view keeps alive. Returning this is safe.
auto evensOfSixNumbers() {
    return std::vector<int>{1, 2, 3, 4, 5, 6}
         | std::views::filter([](int x){ return x % 2 == 0; });
}

int main() {
    std::cout << "safe (owning view):";
    for (int x : evensOfSixNumbers()) std::cout << " " << x;   // 2 4 6
    std::cout << "\n";

    // BAD (described, not compiled or run, because it is undefined behaviour):
    //   auto badView() {
    //       std::vector<int> local = {1, 2, 3, 4};
    //       return local | std::views::filter(...);   // ref_view to `local`
    //   }                                             // local dies here -> DANGLES
    // Piping an LVALUE makes a ref_view that only REFERENCES the container; if the
    // container is a local, the returned view points at freed memory. The fix is
    // to make the view OWN its data -- pipe an rvalue, as above -- or return the
    // container and build the view at the call site.
    std::cout << "done\n";
    return 0;
}
