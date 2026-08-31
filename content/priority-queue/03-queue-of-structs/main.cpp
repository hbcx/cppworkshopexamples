// Give priority_queue a comparator that ranks a struct by one field and it
// becomes a scheduler: top() is always the highest-priority item. The comparator
// returns whether its first argument ranks BELOW the second.

#include <iostream>
#include <queue>
#include <string>
#include <vector>

struct Task {
    int priority;
    std::string name;
};

struct ByPriority {
    bool operator()(const Task& a, const Task& b) const {
        return a.priority < b.priority;   // lower priority ranks below -> top() is highest
    }
};

int main() {
    std::priority_queue<Task, std::vector<Task>, ByPriority> tasks;
    tasks.push({2, "send-email"});
    tasks.push({5, "fire-alert"});
    tasks.push({1, "write-log"});
    tasks.push({4, "run-backup"});
    tasks.push({3, "make-report"});

    std::cout << "running by priority:\n";
    while (!tasks.empty()) {
        std::cout << "  [" << tasks.top().priority << "] " << tasks.top().name << '\n';
        tasks.pop();
    }
}
