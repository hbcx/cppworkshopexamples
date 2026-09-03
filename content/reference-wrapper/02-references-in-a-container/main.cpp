#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

struct Task {
    std::string name;
    int priority;
};

int main() {
    Task t1{"backup", 3};
    Task t2{"deploy", 1};
    Task t3{"report", 2};

    // You cannot make a std::vector<Task&> -- references are not objects. A
    // vector of std::reference_wrapper<Task> holds references to existing Tasks
    // without copying them.
    std::vector<std::reference_wrapper<Task>> queue{t1, t2, t3};

    // Sort the REFERENCES by priority; the Task objects themselves never move.
    std::sort(queue.begin(), queue.end(),
              [](const Task& a, const Task& b){ return a.priority < b.priority; });

    std::cout << "by priority:";
    for (const Task& t : queue) std::cout << " " << t.name;
    std::cout << "\n";   // deploy report backup

    // Mutating through a wrapper changes the original object.
    queue.front().get().name += "!";
    std::cout << "t2 is now " << t2.name << "\n";   // deploy!
    return 0;
}
