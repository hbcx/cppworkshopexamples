// Anti-pattern: expecting equal-priority items to keep their arrival order.
//
// A heap is not stable: among equal priorities, the pop order is unspecified.
// To get FIFO among equals, add a sequence number and break ties by it, so no
// two elements compare equal and the order is fully defined.

#include <iostream>
#include <queue>
#include <string>
#include <vector>

struct Task {
    int priority;
    unsigned seq;        // increases on every push -- the tie-breaker
    std::string name;
};

struct Order {
    bool operator()(const Task& a, const Task& b) const {
        if (a.priority != b.priority) return a.priority < b.priority;  // higher priority first
        return a.seq > b.seq;   // equal priority: smaller seq ranks higher -> FIFO
    }
};

int main() {
    std::priority_queue<Task, std::vector<Task>, Order> tasks;
    unsigned seq = 0;
    tasks.push({5, seq++, "five-a"});
    tasks.push({5, seq++, "five-b"});
    tasks.push({9, seq++, "nine"});
    tasks.push({5, seq++, "five-c"});

    std::cout << "priority then FIFO among equals:\n";
    while (!tasks.empty()) {
        std::cout << "  [" << tasks.top().priority << "] " << tasks.top().name << '\n';
        tasks.pop();
    }
}
