#include <print>

int main() {
    // Anti-pattern: interleaving std::print with std::cout. std::print writes
    // through C stdio (the FILE* stdout), while std::cout has its OWN buffer.
    // If the two are not synchronised, their output can appear in a different
    // order than the code wrote it.
    //
    //   std::cout << "A ";        // C++ stream buffer
    //   std::print("B ");         // C stdio buffer  -> order not guaranteed
    //
    // By default the C++ streams ARE synced with C stdio, so it usually works --
    // but if you called sync_with_stdio(false) for speed, mixing them breaks.
    // The clean rule: pick ONE output path. Here we use std::print throughout.
    std::print("using ");
    std::print("only ");
    std::println("std::print, so the order is well-defined");
    return 0;
}
