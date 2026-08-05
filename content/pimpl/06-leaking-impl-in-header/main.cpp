#include "clean.hpp"
#include "leaky.hpp"

#include <iostream>

int main() {
    LeakyCache leaky;
    leaky.put("a");
    leaky.put("b");
    std::cout << "[leaky] size=" << leaky.size()
              << " (header pulled in <vector> and exposes items_)\n";

    CleanCache clean;
    clean.put("x");
    clean.put("y");
    clean.put("z");
    std::cout << "[clean] size=" << clean.size()
              << " (header hides internals behind Impl)\n";
}
