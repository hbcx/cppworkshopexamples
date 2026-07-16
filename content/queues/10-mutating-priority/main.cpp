#include <iostream>
#include <queue>
#include <string>
#include <vector>

struct Task {
    std::string name;
    int priority;
};

// A scheduler queues POINTERS, so the tasks themselves stay editable elsewhere.
// That is the usual way this trap is set: nothing looks suspicious.
struct ByPriority {
    bool operator()(const Task* a, const Task* b) const { return a->priority < b->priority; }
};
using Scheduler = std::priority_queue<Task*, std::vector<Task*>, ByPriority>;

static void drain(Scheduler pq, const char* label) {
    std::cout << label;
    while (!pq.empty()) {
        std::cout << ' ' << pq.top()->name << '(' << pq.top()->priority << ')';
        pq.pop();
    }
    std::cout << '\n';
}

int main() {
    Task backup{"backup", 1};
    Task report{"report", 2};
    Task deploy{"deploy", 3};

    Scheduler pq;
    pq.push(&backup);
    pq.push(&report);
    pq.push(&deploy);
    drain(pq, "as queued:          ");

    // Anti-pattern: the task is already in the queue, and its priority changes.
    // The heap was arranged on the way IN and nothing re-arranges it now.
    backup.priority = 99;   // "urgent!" -- but the queue has no idea

    // top() is still 'deploy'. The queue is not broken in the crashing sense --
    // it is just answering a question about an ordering that no longer exists.
    std::cout << "after backup -> 99: top is " << pq.top()->name << '('
              << pq.top()->priority << ')' << "  <- expected backup(99)\n";
    drain(pq, "wrong order:        ");

    // The same trap with a value-typed queue is written like this -- top()
    // returns a const reference precisely to make you stop and think:
    //   const_cast<Task&>(pq.top()).priority = 99;   // compiles, still wrong

    // Fix: a priority is immutable while the element is queued. Set it while the
    // task is OUT of the queue; push then places it correctly in O(log n). To
    // reprioritise something already queued, pop it out first and push it back.
    Scheduler correct;
    correct.push(&report);
    correct.push(&deploy);
    backup.priority = 99;         // 'backup' is not in this queue yet -- free to edit
    correct.push(&backup);        // enters the heap with its final priority
    std::cout << "priority set before push: top is " << correct.top()->name << '('
              << correct.top()->priority << ")\n";
    drain(correct, "right order:        ");
    return 0;
}
