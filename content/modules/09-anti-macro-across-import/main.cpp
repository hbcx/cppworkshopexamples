#include <iostream>

import config;

int main() {
    // SECRET_ANSWER is NOT visible here -- modules do not export macros. We use the
    // exported function and constant instead.
    std::cout << "answer()         = " << answer() << "\n";
    std::cout << "answer_constant  = " << answer_constant << "\n";
    return 0;
}
