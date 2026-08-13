#include <algorithm>
#include <iostream>

int main() {
    // std::min / std::max take VALUES -- two of them, or an initializer_list --
    // and return the smaller / larger one. They are not a range and not an
    // iterator; they answer "which of these values is bigger".
    std::cout << "min(3, 8):         " << std::min(3, 8) << '\n';
    std::cout << "max({4, 9, 2, 7}): " << std::max({4, 9, 2, 7}) << '\n';

    // std::clamp (C++17) bounds a value to a range: it returns lo if v < lo, hi if
    // v > hi, otherwise v unchanged. It replaces the easy-to-misorder idiom
    // std::max(lo, std::min(v, hi)).
    int lo = 0, hi = 100;
    std::cout << "clamp(-5):  " << std::clamp(-5, lo, hi) << '\n';
    std::cout << "clamp(50):  " << std::clamp(50, lo, hi) << '\n';
    std::cout << "clamp(150): " << std::clamp(150, lo, hi) << '\n';
    return 0;
}
