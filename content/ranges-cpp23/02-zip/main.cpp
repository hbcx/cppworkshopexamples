#include <ranges>
#include <vector>
#include <string>
#include <iostream>

int main() {
    std::vector<std::string> names{"Ada", "Linus", "Grace"};
    std::vector<int> scores{95, 82, 88};

    // Walk two (or more) ranges in lockstep. Each step yields a tuple with one
    // element per range -- no index variable, no bounds bookkeeping.
    for (auto [name, score] : std::views::zip(names, scores))
        std::cout << name << ": " << score << '\n';

    // The tuple holds REFERENCES into the ranges, so you can write through it.
    for (auto [name, score] : std::views::zip(names, scores)) {
        (void)name;      // not needed here; we only bump the score
        score += 5;
    }
    std::cout << "after bonus: "
              << scores[0] << ' ' << scores[1] << ' ' << scores[2] << '\n';
    return 0;
}
