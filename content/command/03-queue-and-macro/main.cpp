#include <functional>
#include <iostream>
#include <utility>
#include <vector>

// Because a command is an object, it can be STORED and run later, or grouped. A
// queue collects commands and runs them in order; a macro is a single command
// that runs several -- Command composing with itself, a Composite of commands.
using Command = std::function<void()>;

class Queue {
public:
    void add(Command c) { commands_.push_back(std::move(c)); }
    void run() {
        for (auto& c : commands_) c();
        commands_.clear();
    }
private:
    std::vector<Command> commands_;
};

// A macro command: several commands bundled and executed as one.
Command macro(std::vector<Command> steps) {
    return [steps] {
        for (auto& s : steps) s();
    };
}

int main() {
    Queue queue;
    queue.add([] { std::cout << "step 1\n"; });
    queue.add(macro({
        [] { std::cout << "  macro a\n"; },
        [] { std::cout << "  macro b\n"; },
    }));
    queue.add([] { std::cout << "step 3\n"; });

    std::cout << "running queue:\n";
    queue.run();     // step 1, then the macro (a, b) as one command, then step 3
    return 0;
}
