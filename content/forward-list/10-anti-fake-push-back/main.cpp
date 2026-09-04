#include <forward_list>
#include <iostream>
#include <iterator>   // std::next

// Count node visits so the O(n^2) is visible without a timer.
static long g_steps = 0;

// A "push_back" for forward_list, the way people fake it: walk to the last node,
// then insert_after it. The walk is what costs -- O(n) every single call.
static void fake_push_back(std::forward_list<int>& fl, int value) {
    if (fl.empty()) {
        fl.push_front(value);
        return;
    }
    auto it = fl.begin();
    while (std::next(it) != fl.end()) { ++it; ++g_steps; }   // hunt for the tail
    fl.insert_after(it, value);
}

int main() {
    const int N = 50;

    // ANTI-PATTERN: every append re-finds the tail -> total is 0+1+...+(N-1).
    g_steps = 0;
    std::forward_list<int> bad;
    for (int i = 0; i < N; ++i) fake_push_back(bad, i);
    std::cout << "fake push_back: " << N << " appends took "
              << g_steps << " tail-walk steps\n";
    // The first append is push_front and the second finds the tail for free, so
    // the walk cost is 0+0+1+2+...+(N-2) = (N-1)(N-2)/2 -- quadratic in N.
    std::cout << "matches (N-1)(N-2)/2 = " << (long)(N - 1) * (N - 2) / 2 << "\n";

    // FIX: keep a trailing tail iterator (see example 06) -- each append is O(1)
    // and no node is ever re-walked.
    g_steps = 0;
    std::forward_list<int> good;
    auto tail = good.before_begin();
    for (int i = 0; i < N; ++i) tail = good.insert_after(tail, i);
    std::cout << "trailing-tail build: " << g_steps << " tail-walk steps\n";
    return 0;
}
