#include <generator>
#include <iostream>
#include <ranges>

// A generator is an input_range, so it flows straight into the ranges views. The
// generator is the lazy source; filter and take are lazy stages on top of it.
std::generator<int> naturals() {
    for (int i = 1; ; ++i)      // infinite; only pulled as far as needed
        co_yield i;
}

int main() {
    auto pipeline = naturals()
                  | std::views::filter([](int n) { return n % 2 == 0; })
                  | std::views::take(5);

    std::cout << "first 5 even numbers:";
    for (int v : pipeline)
        std::cout << " " << v;
    std::cout << "\n";
    return 0;
}
