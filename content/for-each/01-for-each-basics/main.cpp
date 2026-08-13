#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{10, 20, 30, 40};

    // for_each applies a callable to every element of a range, in order. The
    // callable is invoked once per element, receiving the element as its argument.
    std::for_each(v.begin(), v.end(), [](int x) {
        std::cout << "value: " << x << '\n';
    });
    return 0;
}
