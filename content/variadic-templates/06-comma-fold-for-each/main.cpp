#include <iostream>

// A fold over the COMMA operator runs a statement for EACH element of the pack --
// the modern "do X for each argument", left to right, guaranteed.
template <class... Ts>
void printEach(const Ts&... args) {
    int i = 0;
    ((std::cout << "arg " << i++ << " = " << args << "\n"), ...);   // comma fold
}

int total = 0;
template <class... Ts>
void addAll(Ts... args) {
    ((total += args), ...);   // one += per element (a side effect, not a value to combine)
}

int main() {
    printEach("hello", 42, 3.14);

    addAll(10, 20, 30, 40);
    std::cout << "total = " << total << "\n";   // 100
    return 0;
}
