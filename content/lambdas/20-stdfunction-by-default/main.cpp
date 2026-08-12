#include <iostream>
#include <functional>
#include <vector>

// BAD: taking std::function forces type erasure -- a heap allocation for the
// closure and an indirect call through a pointer -- even when the caller passes a
// simple lambda that could have been inlined.
static long sumWithFunction(const std::vector<int>& v, std::function<int(int)> f) {
    long total = 0;
    for (int x : v) total += f(x);
    return total;
}

// GOOD: a template parameter takes the lambda by its real type, so the call
// inlines and nothing is allocated. Same call site, no overhead.
template <class F>
long sumWith(const std::vector<int>& v, F f) {
    long total = 0;
    for (int x : v) total += f(x);
    return total;
}

int main() {
    std::vector<int> v{1, 2, 3, 4, 5};
    auto square = [](int x) { return x * x; };

    std::cout << "via std::function (type-erased): " << sumWithFunction(v, square) << '\n';   // 55
    std::cout << "via template (zero overhead):    " << sumWith(v, square) << '\n';           // 55

    // Use std::function only when you must store different callable types together
    // or fix a callable type across a boundary; for just passing a lambda into a
    // function, a template (or auto) parameter is the default, faster choice.
    return 0;
}
