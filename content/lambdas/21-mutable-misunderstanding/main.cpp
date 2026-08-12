#include <iostream>

int main() {
    int score = 100;

    // Misconception 1: `mutable` lets the lambda change the OUTSIDE variable.
    // It does not -- a by-value capture is a COPY; mutable only lets the body
    // change that copy.
    auto penalize = [score]() mutable {
        score -= 10;      // changes the closure's copy, not the outer score
        return score;
    };
    std::cout << "penalize(): " << penalize() << '\n';        // 90
    std::cout << "outer score still: " << score << '\n';      // 100 -- untouched

    // Misconception 2: each call starts again from the captured value.
    // It does not -- the copy PERSISTS between calls, so the lambda accumulates.
    std::cout << "penalize() again: " << penalize() << '\n';  // 80, not 90
    std::cout << "penalize() again: " << penalize() << '\n';  // 70

    // If you wanted to affect the outside, capture by reference; if you wanted a
    // fresh value each call, pass it as an argument. mutable does neither.
    return 0;
}
