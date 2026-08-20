#include <format>
#include <iostream>

int main() {
    // The spec comes after a colon:  {:[fill][align][width][.precision][type]}
    // align is < left, > right, ^ center.
    std::cout << std::format("[{:>8}]\n", "right");    // [   right]
    std::cout << std::format("[{:<8}]\n", "left");     // [left    ]
    std::cout << std::format("[{:^8}]\n", "mid");      // [  mid   ]

    // A fill character goes before the align.
    std::cout << std::format("[{:*^8}]\n", "mid");     // [**mid***]

    // On a string, precision TRUNCATES to that many characters.
    std::cout << std::format("[{:.3}]\n", "truncated"); // [tru]

    // Width + precision on a number: zero-fill to width 8, two decimals.
    std::cout << std::format("[{:08.2f}]\n", 3.14159);  // [00003.14]
    return 0;
}
