#include <generator>
#include <iostream>

// A counter proving the generator only computes what is pulled.
long g_produced = 0;

std::generator<long> fibonacci() {
    long a = 0, b = 1;
    while (true) {              // infinite, but lazy: it advances only when pulled
        ++g_produced;
        co_yield a;
        long next = a + b;
        a = b;
        b = next;
    }
}

int main() {
    std::cout << "first 10 fibonacci:";
    int count = 0;
    for (long v : fibonacci()) {
        std::cout << " " << v;
        if (++count == 10)
            break;             // stop pulling; the generator simply suspends
    }
    std::cout << "\n";
    std::cout << "values actually produced = " << g_produced << "\n";
    return 0;
}
