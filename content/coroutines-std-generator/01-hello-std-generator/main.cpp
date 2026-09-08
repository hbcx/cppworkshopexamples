#include <generator>
#include <iostream>

// The whole hand-rolled Generator<T> from the first coroutine section -- promise
// type, iterator, move-only handle ownership -- collapses into one library type.
// A function that co_yields and returns std::generator<T> is a ready generator.
std::generator<int> range(int first, int last) {
    for (int i = first; i < last; ++i)
        co_yield i;
}

int main() {
    std::cout << "range 1..5:";
    for (int v : range(1, 6))       // std::generator is directly range-for-able
        std::cout << " " << v;
    std::cout << "\n";
    return 0;
}
