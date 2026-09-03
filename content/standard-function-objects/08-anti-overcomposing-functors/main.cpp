#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

int main() {
    using namespace std::placeholders;
    std::vector<int> v{1, 2, 3, 4};

    // ANTI-PATTERN: composing standard function objects with std::bind to build a
    // simple operation. This "double each element" reads backwards and drags in
    // both the <functional> machinery and the placeholders namespace.
    std::vector<int> a(v.size());
    std::transform(v.begin(), v.end(), a.begin(),
                   std::bind(std::multiplies<int>(), _1, 2));
    std::cout << "via bind+multiplies:";
    for (int x : a) std::cout << " " << x;
    std::cout << "\n";   // 2 4 6 8

    // FIX: a lambda states the operation directly and inlines cleanly.
    std::vector<int> b(v.size());
    std::transform(v.begin(), v.end(), b.begin(), [](int x){ return x * 2; });
    std::cout << "via lambda:";
    for (int x : b) std::cout << " " << x;
    std::cout << "\n";   // 2 4 6 8

    // Standard function objects earn their keep as a whole NAMED operation passed
    // to an algorithm or a container (std::greater as a comparator, std::plus as
    // a reduce op) -- not glued together with bind to fake a lambda. The old
    // helpers for that (std::bind2nd, std::ptr_fun, std::mem_fun) were deprecated
    // in C++11 and removed in C++17 for exactly this reason.
    return 0;
}
