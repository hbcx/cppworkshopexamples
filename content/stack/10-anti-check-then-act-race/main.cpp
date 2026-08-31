// Anti-pattern: empty() then top()/pop() on a stack shared across threads.
//
// The check and the take are separate calls, so another thread can empty the
// stack in the gap and the pop hits an empty stack. Hold one lock around the
// whole check-and-take. (Shown single-threaded to stay deterministic.)

#include <iostream>
#include <mutex>
#include <stack>

std::stack<int> shared;
std::mutex m;

// BAD (race -- described, not run this way):
//   if (!shared.empty()) {          // thread A and B both pass this
//       int x = shared.top();       // ... then both take -- one hits empty
//       shared.pop();
//   }

// GOOD: one lock covers the whole check-and-take.
static bool tryPop(int& out) {
    std::lock_guard<std::mutex> lock(m);
    if (shared.empty()) return false;
    out = shared.top();
    shared.pop();
    return true;
}

int main() {
    shared.push(1);
    shared.push(2);

    int value = 0;
    while (tryPop(value)) {
        std::cout << "popped " << value << '\n';
    }
    std::cout << "tryPop returned false on the empty stack -- no undefined pop\n";
}
