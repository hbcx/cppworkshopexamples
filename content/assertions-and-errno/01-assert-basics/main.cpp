// assert(expr) from <cassert> checks a condition at run time. If expr is false it
// prints file, line and the expression and calls std::abort; if true it does
// nothing. Use it to state facts that must hold when the code is correct --
// preconditions, postconditions, invariants -- so a broken assumption stops the
// program at the exact spot instead of corrupting data downstream.
#include <iostream>
#include <cassert>

int average(const int* data, int count) {
    assert(data != nullptr && "data must not be null");     // precondition
    assert(count > 0 && "count must be positive");          // precondition
    int sum = 0;
    for (int i = 0; i < count; ++i) sum += data[i];
    int avg = sum / count;
    assert(avg >= 0 && "this demo uses non-negative data"); // sanity check
    return avg;
}

int main() {
    int values[] = {10, 20, 30};

    // All preconditions hold, so every assert passes silently.
    std::cout << "average: " << average(values, 3) << '\n';

    // A call like average(nullptr, 0) would fire the first assert and abort with
    // a message naming this file and line. We do not run it: an abort stops the
    // program (and its output is not something to capture).
    std::cout << "done\n";
    return 0;
}
