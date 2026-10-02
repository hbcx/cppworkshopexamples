#include <iostream>
#include <ranges>
#include <string>
#include <string_view>

int main() {
    std::string text = "a,bb,ccc";

    // views::split keeps each piece as a subrange of the ORIGINAL range, so over
    // contiguous input each piece is contiguous and becomes a string_view.
    std::cout << "split:";
    for (auto piece : text | std::views::split(',')) {
        std::string_view sv(piece.begin(), piece.end());
        std::cout << " [" << sv << ']';
    }
    std::cout << '\n';   // [a] [bb] [ccc]

    // views::lazy_split is the general form: it works on input-only ranges and
    // pattern delimiters, but each piece is a LAZY range, not necessarily
    // contiguous, so you iterate it element by element -- no string_view shortcut.
    std::cout << "lazy_split:";
    for (auto piece : text | std::views::lazy_split(',')) {
        std::cout << " [";
        for (char ch : piece) std::cout << ch;
        std::cout << ']';
    }
    std::cout << '\n';   // [a] [bb] [ccc]

    // Prefer split for contiguous input you want to slice into string_views;
    // reach for lazy_split when the input is input-only or the delimiter is a
    // pattern split cannot handle.
    return 0;
}
