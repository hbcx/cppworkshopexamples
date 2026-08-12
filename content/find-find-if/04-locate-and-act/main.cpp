#include <algorithm>
#include <vector>
#include <string>
#include <iostream>

struct Task {
    int id;
    std::string title;
    bool done;
};

int main() {
    std::vector<Task> tasks{
        {1, "write", false},
        {2, "review", false},
        {3, "ship", false},
    };

    // find_if returns the position, so you act on it directly -- read fields,
    // modify in place, or erase at that spot. No separate index lookup.
    auto it = std::find_if(tasks.begin(), tasks.end(),
        [](const Task& t){ return t.id == 2; });
    if (it != tasks.end()) {
        it->done = true;                        // modify through the iterator
        std::cout << "marked done: " << it->title << '\n';
    }

    // Erase the found element at its position -- erase takes the iterator directly.
    auto ship = std::find_if(tasks.begin(), tasks.end(),
        [](const Task& t){ return t.title == "ship"; });
    if (ship != tasks.end())
        tasks.erase(ship);

    std::cout << "remaining: ";
    for (const auto& t : tasks)
        std::cout << t.id << (t.done ? "(done) " : " ");
    std::cout << '\n';
    return 0;
}
