#include <iostream>

// The caller must use the return value; ignoring it warns (an error under -Werror).
[[nodiscard]] int compute() { return 42; }

// A whole type can be nodiscard: every function returning it is then nodiscard too.
struct [[nodiscard]] Status {
    bool ok;
};
Status check(int x) { return {x > 0}; }

// C++20: attach a reason, shown in the diagnostic.
[[nodiscard("handle the parsed count")]] int parse() { return 7; }

int main() {
    int r = compute();                 // result used -> fine
    std::cout << "compute() = " << r << "\n";

    if (check(5).ok)                   // Status used in a condition -> fine
        std::cout << "check passed\n";

    // To discard a nodiscard result ON PURPOSE, cast to void -- it says you meant it.
    (void)parse();
    std::cout << "parse() result intentionally discarded\n";

    // NOTE: writing `compute();` as a bare statement would warn:
    //   ignoring return value of 'int compute()', declared with attribute nodiscard
    // and fail the build under -Werror.
    return 0;
}
