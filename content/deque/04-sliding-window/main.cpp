// A sliding window of the last N values: push_back the newest, pop_front the
// oldest once the window is full. Both ends are O(1), so a deque is the natural
// fit -- a vector would pay O(n) per erase(begin()).

#include <deque>
#include <iostream>

int main() {
    const std::size_t windowSize = 3;
    std::deque<int> window;

    auto feed = [&](int value) {
        window.push_back(value);
        if (window.size() > windowSize) window.pop_front();   // drop the oldest
    };

    for (int value : {10, 20, 30, 40, 50}) {
        feed(value);

        int sum = 0;
        for (int v : window) sum += v;

        std::cout << "added " << value << " -> window [";
        for (std::size_t i = 0; i < window.size(); ++i)
            std::cout << (i ? " " : "") << window[i];
        std::cout << "]  avg = " << static_cast<double>(sum) / window.size() << '\n';
    }
}
