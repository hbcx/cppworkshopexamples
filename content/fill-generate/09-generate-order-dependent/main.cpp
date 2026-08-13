#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    // A generator whose output depends on being called strictly left-to-right:
    // each value is built from the previous ones held in captured state (a
    // Fibonacci run). This WORKS with the plain, sequential generate...
    std::vector<int> fib(8);
    int a = 0, b = 1;
    std::generate(fib.begin(), fib.end(), [&a, &b]() {
        int cur = a;
        int next = a + b;
        a = b;
        b = next;
        return cur;
    });
    std::cout << "fibonacci: ";
    for (int x : fib) std::cout << x << ' ';
    std::cout << '\n';

    // ...but the generator now carries ORDER-DEPENDENT STATE: it assumes each call
    // happens once, in order. That assumption breaks under the parallel overload
    // std::generate(std::execution::par, ...), where the calls may run in any
    // order or at the same time -- the captured a/b would be raced and the result
    // garbage. A generator is safest when each call is INDEPENDENT of the others;
    // for a recurrence like this, an explicit loop states the dependency openly
    // instead of hiding it inside a capture that only works sequentially.
    return 0;
}
