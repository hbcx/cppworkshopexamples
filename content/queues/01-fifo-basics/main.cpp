#include <iostream>
#include <queue>
#include <string>

int main() {
    std::queue<std::string> line;   // people waiting at a counter

    line.push("alice");             // push always appends at the BACK
    line.push("bob");
    line.push("carol");

    std::cout << "size  = " << line.size() << '\n';
    std::cout << "front = " << line.front() << "   (the oldest, served next)\n";
    std::cout << "back  = " << line.back() << "   (the newest, just arrived)\n";

    // Serving the queue: read front(), then pop() it. Repeat until empty.
    std::cout << "serving:";
    while (!line.empty()) {         // front()/pop() on an empty queue is UB
        std::cout << ' ' << line.front();
        line.pop();
    }
    std::cout << '\n';
    std::cout << "empty now? " << (line.empty() ? "yes" : "no") << '\n';
    return 0;
}
