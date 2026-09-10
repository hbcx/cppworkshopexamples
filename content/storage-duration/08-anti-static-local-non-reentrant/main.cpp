#include <iostream>
#include <string>

// ANTI-PATTERN: keeping working state in a static local makes the function
// non-reentrant. Every call shares the one buffer, so a nested (here recursive)
// call clobbers the state the outer call was still using. The same shared state
// also makes the function unsafe to call from multiple threads.
std::string indentBroken(int depth) {
    static std::string pad;      // ONE buffer shared by all calls
    pad.clear();
    for (int i = 0; i < depth; ++i)
        pad += "  ";
    if (depth > 0)
        (void)indentBroken(depth - 1);   // the inner call rewrites pad
    return pad;                  // pad now holds whatever the deepest call left
}

// FIX: keep the state in a local variable, so every call -- and every thread --
// has its own.
std::string indentFixed(int depth) {
    std::string pad;
    for (int i = 0; i < depth; ++i)
        pad += "  ";
    if (depth > 0)
        (void)indentFixed(depth - 1);
    return pad;
}

int main() {
    std::cout << "broken indent(3) width = " << indentBroken(3).size()
              << "  (wanted 6)\n";
    std::cout << "fixed  indent(3) width = " << indentFixed(3).size()
              << "  (wanted 6)\n";
    return 0;
}
