#include <iostream>
#include <string>
#include <vector>

int main() {
    // Immediately-Invoked lambda (IIFE): define and call it in one expression,
    // [] { ... }(). Its result initializes a const that needs real logic to
    // compute -- so the variable stays const instead of being declared mutable
    // and then patched up.
    const int level = [] {
        int configured = 3;          // pretend this came from a config file
        if (configured > 5) return 2;
        if (configured > 0) return 1;
        return 0;
    }();
    std::cout << "const level = " << level << '\n';   // 1

    // The same for a value built up in a loop: compute it in place, keep it const.
    const std::string csv = [] {
        std::vector<std::string> parts{"a", "b", "c"};
        std::string out;
        for (const auto& p : parts) {
            if (!out.empty()) out += ',';
            out += p;
        }
        return out;
    }();
    std::cout << "const csv = " << csv << '\n';   // a,b,c
    return 0;
}
