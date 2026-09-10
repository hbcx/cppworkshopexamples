#include <iostream>
#include <utility>

int main() {
    int x = 10;
    int old = std::exchange(x, 20);   // set x to 20, return its PREVIOUS value
    std::cout << "x = " << x << ", old = " << old << "\n";

    // A common idiom: flip a flag and act on what it WAS, in one expression.
    bool firstRun = true;
    for (int i = 0; i < 3; ++i) {
        if (std::exchange(firstRun, false))
            std::cout << "iteration " << i << ": first run\n";
        else
            std::cout << "iteration " << i << ": later run\n";
    }
    return 0;
}
