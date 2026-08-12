#include <algorithm>
#include <vector>
#include <iterator>
#include <iostream>

int main() {
    std::vector<int> in{1, 2, 3, 4, 5, 6};
    // GOAL: keep only the even numbers, doubled -> 4 8 12.

    // BAD: transform maps 1:1 -- exactly one output per input, and it cannot drop
    // any. There is no way to skip an element; returning the input unchanged for the
    // odds just leaves them in the output.
    std::vector<int> mapped;
    std::transform(in.begin(), in.end(), std::back_inserter(mapped),
        [](int x){ return x % 2 == 0 ? x * 2 : x; });   // odds leak through
    std::cout << "transform only (odds leak): ";
    for (int x : mapped) std::cout << x << ' ';
    std::cout << '\n';   // 1 4 3 8 5 12 -- not what we wanted

    // FIX: filtering is a separate step. copy_if selects, then transform maps the
    // survivors.
    std::vector<int> evens;
    std::copy_if(in.begin(), in.end(), std::back_inserter(evens),
        [](int x){ return x % 2 == 0; });
    std::vector<int> result;
    std::transform(evens.begin(), evens.end(), std::back_inserter(result),
        [](int x){ return x * 2; });
    std::cout << "copy_if then transform:     ";
    for (int x : result) std::cout << x << ' ';
    std::cout << '\n';   // 4 8 12
    return 0;
}
