#include <iostream>
#include <ranges>
#include <vector>

namespace views = std::views;

int main() {
    std::vector<int> v{10, 20, 30, 40};

    // views::all -- wrap a range as a non-owning view. The pipe operator inserts
    // this implicitly at the front of a pipeline; here it is written out.
    auto a = views::all(v);
    std::cout << "all size: " << std::ranges::size(a) << '\n';          // 4

    // views::counted(it, n) -- a view of the first n elements from an iterator,
    // the way to adapt an iterator-plus-length (or pointer-plus-length) pair.
    std::cout << "counted:";
    for (int x : views::counted(v.begin() + 1, 2)) std::cout << ' ' << x;  // 20 30
    std::cout << '\n';

    // views::single(x) -- a view of exactly one element.
    for (int x : views::single(99)) std::cout << "single: " << x << '\n';  // 99

    // views::empty<T> -- a typed view of no elements.
    std::cout << "empty size: " << std::ranges::size(views::empty<int>) << '\n';   // 0

    // single and empty are the natural base cases of a conditional pipeline: a
    // branch yields one value or none while keeping the same view-shaped type.
    return 0;
}
