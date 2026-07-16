#include <algorithm>
#include <deque>
#include <iostream>
#include <queue>

// Inspecting a queue without disturbing it: take a COPY and drain that.
// O(n) copies, so this belongs in logging and tests, not in a hot loop.
static void dump(std::queue<int> copy, const char* label) {
    std::cout << label;
    while (!copy.empty()) {
        std::cout << ' ' << copy.front();
        copy.pop();
    }
    std::cout << '\n';
}

int main() {
    std::queue<int> q;
    for (int i = 1; i <= 5; ++i) {
        q.push(i * 10);
    }

    // None of these compile -- the adaptor has no iterators at all:
    //   for (int x : q) { ... }              // no begin()/end()
    //   std::find(q.begin(), q.end(), 30);   // same reason
    //   int x = q[2];                        // no operator[]
    // Only the two ends are visible:
    std::cout << "front = " << q.front() << ", back = " << q.back()
              << ", size = " << q.size() << '\n';

    dump(q, "drained copy:");
    std::cout << "original untouched, size = " << q.size() << '\n';

    // If you need to look inside for real, use the container itself. A deque has
    // the same O(1) push_back/pop_front as the queue, plus iteration.
    std::deque<int> d(q.size());
    for (std::size_t i = 0; !q.empty(); ++i) {
        d[i] = q.front();
        q.pop();
    }
    std::cout << "deque can search: found 30? "
              << (std::find(d.begin(), d.end(), 30) != d.end() ? "yes" : "no") << '\n';
    std::cout << "deque can index:  d[1] = " << d[1] << '\n';
    return 0;
}
