#include <format>
#include <iostream>

int main() {
    int width = 10;
    int prec = 3;

    // A nested {} inside the spec takes the width from an argument at run time.
    std::cout << std::format("[{:>{}}]\n", "x", width);        // [         x]

    // Same idea for precision: .{} pulls it from the next argument.
    std::cout << std::format("[{:.{}f}]\n", 3.14159, prec);    // [3.142]

    // Both together, using positional indices to pick value, width and precision.
    std::cout << std::format("[{0:>{1}.{2}f}]\n", 2.71828, width, prec);  // [     2.718]
    return 0;
}
