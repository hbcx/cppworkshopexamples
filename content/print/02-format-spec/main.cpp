#include <print>

int main() {
    // print reuses std::format's spec, after a ':' in the placeholder.
    // Alignment and width: '<' left, '>' right, '^' center, with a fill char.
    std::println("[{:<8}]", "left");
    std::println("[{:>8}]", "right");
    std::println("[{:^8}]", "mid");
    std::println("[{:*^8}]", "pad");     // fill with '*'

    // Numbers: precision, sign, base with prefix and zero-pad.
    std::println("pi = {:.3f}", 3.14159);   // 3.142
    std::println("hex = {:#06x}", 255);     // 0x00ff
    std::println("signed = {:+}", 42);      // +42

    // Argument indices let you reorder or reuse arguments.
    std::println("{0} {1} {0}", "a", "b");  // a b a
    return 0;
}
