#include <iostream>

import account;

int main() {
    std::cout << "totalCost(20) = " << totalCost(20) << "\n";   // 20 + (20+1) = 41

    // applyFee is private to the module: it was not exported, so it is invisible
    // here. Writing `applyFee(5);` would fail with 'applyFee' not declared.
    return 0;
}
