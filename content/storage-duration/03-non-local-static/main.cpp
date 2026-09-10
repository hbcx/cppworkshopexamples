#include <iostream>

struct Banner {
    Banner() { std::cout << "  Banner constructed (before main runs)\n"; }
};

// Non-local statics. Every one is zero-initialized first, then those needing a
// runtime value are dynamically initialized -- all before main is entered.
Banner g_banner;            // dynamic init: its constructor runs before main
int g_zero;                 // zero-initialized to 0
int g_constant = 6 * 7;     // constant-initialized to 42

int main() {
    std::cout << "main starts\n";
    std::cout << "g_zero = " << g_zero << ", g_constant = " << g_constant << "\n";
    return 0;
}
