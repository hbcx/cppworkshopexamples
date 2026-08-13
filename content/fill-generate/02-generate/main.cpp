#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v(6);

    // generate calls a function that takes NO arguments once per element and stores
    // its result. Here a stateful generator produces the running powers of two by
    // keeping the next value in captured state.
    int next = 1;
    std::generate(v.begin(), v.end(), [&next]() {
        int value = next;
        next *= 2;
        return value;
    });

    std::cout << "powers of two: ";
    for (int x : v) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
