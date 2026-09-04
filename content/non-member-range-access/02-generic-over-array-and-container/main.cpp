#include <iostream>
#include <vector>
#include <string>
#include <iterator>   // std::begin, std::end

// One function, three range shapes: std::vector, a raw array, and std::string.
// Because it goes through std::begin/std::end, it needs no overloads and no
// pointer-plus-length variant for the array.
template <typename Range, typename T>
bool contains(const Range& r, const T& value) {
    for (auto it = std::begin(r); it != std::end(r); ++it)
        if (*it == value)
            return true;
    return false;
}

int main() {
    std::vector<int> ids = {4, 8, 15, 16, 23, 42};
    double temps[] = {19.5, 20.0, 21.5};   // raw array
    std::string word = "iterator";

    std::cout << std::boolalpha;
    std::cout << "ids contains 16:   " << contains(ids, 16) << "\n";
    std::cout << "temps contains 20: " << contains(temps, 20.0) << "\n";
    std::cout << "word contains r:   " << contains(word, 'r') << "\n";
    std::cout << "word contains z:   " << contains(word, 'z') << "\n";
    return 0;
}
