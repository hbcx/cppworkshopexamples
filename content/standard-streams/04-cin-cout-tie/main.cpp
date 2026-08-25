#include <iostream>

int main() {
    // std::cin is TIED to std::cout by default: any input operation on cin first
    // flushes cout. That is why a prompt written to cout (without a newline)
    // appears on screen BEFORE the program blocks waiting for input.
    //
    //   std::cout << "Enter name: ";   // no newline, not flushed on its own
    //   std::cin  >> name;             // cin flushes cout first -> prompt shows
    //
    // tie() returns the currently tied stream; by default cin.tie() == &cout.
    bool tiedToCout = (std::cin.tie() == &std::cout);
    std::cout << "cin tied to cout by default: " << tiedToCout << '\n';   // 1

    // Untie for speed: with no tie, cin no longer flushes cout on every read,
    // which is faster for bulk input -- but then you must flush prompts yourself.
    std::cin.tie(nullptr);
    std::cout << "after untie, cin.tie() is null: "
              << (std::cin.tie() == nullptr) << '\n';   // 1

    // Restore the default tie (good manners if other code relies on it).
    std::cin.tie(&std::cout);
    return 0;
}
