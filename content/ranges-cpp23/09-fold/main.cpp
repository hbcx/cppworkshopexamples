#include <ranges>
#include <algorithm>
#include <vector>
#include <functional>
#include <optional>
#include <iostream>

int main() {
    std::vector<int> v{1, 2, 3, 4, 5};

    // fold_left: reduce left to right from an initial value, returning the result
    // directly. Unlike std::accumulate it is range-based and composes with views.
    int sum = std::ranges::fold_left(v, 0, std::plus{});
    std::cout << "fold_left sum: " << sum << '\n';

    // It folds a pipeline with no intermediate container.
    int sumSquares = std::ranges::fold_left(
        v | std::views::transform([](int x){ return x * x; }), 0, std::plus{});
    std::cout << "sum of squares: " << sumSquares << '\n';

    // fold_left_first uses the FIRST element as the starting value, so there is
    // no result for an empty range -- it returns std::optional (see the anti-pattern).
    std::optional<int> product = std::ranges::fold_left_first(v, std::multiplies{});
    std::cout << "fold_left_first product: " << product.value() << '\n';

    // fold_right folds right to left. With a non-commutative op the direction
    // shows: this computes 1 - (2 - (3 - (4 - (5 - 0)))).
    int r = std::ranges::fold_right(v, 0, std::minus{});
    std::cout << "fold_right with minus: " << r << '\n';
    return 0;
}
