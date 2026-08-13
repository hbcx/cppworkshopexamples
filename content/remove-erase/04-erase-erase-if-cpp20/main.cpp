#include <vector>
#include <iostream>

int main() {
    std::vector<int> a{3, 1, 4, 1, 5, 1, 9};

    // std::erase(container, value) is the C++20 free function that wraps the
    // whole erase-remove idiom in one call: it removes every element equal to
    // the value, shrinks the container, and returns how many it removed.
    auto removed = std::erase(a, 1);
    std::cout << "erase removed " << removed << ", left: ";
    for (int x : a) std::cout << x << ' ';
    std::cout << '\n';

    // std::erase_if(container, pred) is the predicate form -- one call instead
    // of remove_if plus erase. Both work on every standard container.
    std::vector<int> b{10, 3, 25, 4, 7, 40, 2};
    auto n = std::erase_if(b, [](int x) { return x % 2 == 0; });
    std::cout << "erase_if removed " << n << ", left: ";
    for (int x : b) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
