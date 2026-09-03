#include <functional>
#include <iostream>
#include <tuple>
#include <utility>

int main() {
    int score = 5;

    // std::make_tuple DECAYS its arguments -- it copies each by value, so plain
    // make_tuple(score) stores an int, disconnected from score.
    auto byValue = std::make_tuple(score);
    score = 99;
    std::cout << "stored by value: " << std::get<0>(byValue) << "\n";   // 5 -- a copy

    // Wrap in std::ref and make_tuple stores an int& instead: a reference_wrapper
    // decays to a real reference inside the tuple.
    int live = 5;
    auto byRef = std::make_tuple(std::ref(live));   // tuple<int&>
    live = 99;
    std::cout << "stored by ref:   " << std::get<0>(byRef) << "\n";     // 99 -- the live object

    // std::make_pair does the same decay.
    auto p = std::make_pair(std::ref(live), 0);     // pair<int&, int>
    live = 123;
    std::cout << "pair first:      " << p.first << "\n";                // 123

    // This is exactly the rule std::thread, std::async and std::bind follow: they
    // copy their arguments, and std::ref is how you hand them a reference instead.
    return 0;
}
