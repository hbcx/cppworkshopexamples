#include <iostream>
#include <queue>
#include <string>
#include <utility>

struct Counters {
    int ctor = 0;
    int copy = 0;
    int move = 0;
};
static Counters counters;

struct Message {
    std::string topic;
    int priority;

    Message(std::string t, int p) : topic(std::move(t)), priority(p) { ++counters.ctor; }
    Message(const Message& other) : topic(other.topic), priority(other.priority) { ++counters.copy; }
    Message(Message&& other) noexcept
        : topic(std::move(other.topic)), priority(other.priority) { ++counters.move; }
};

static void report(const char* label) {
    std::cout << label << ": ctor=" << counters.ctor << " copy=" << counters.copy
              << " move=" << counters.move << '\n';
    counters = Counters();
}

int main() {
    std::queue<Message> q;

    // push needs a finished Message first, so the temporary is built and then
    // moved into the queue: two constructions for one element.
    q.push(Message("alerts", 1));
    report("push(temporary)  ");

    // A named object cannot be moved from implicitly -- it is an lvalue, so push
    // COPIES it. std::move(named) would turn this back into a move.
    Message named("metrics", 2);
    counters = Counters();          // ignore the cost of building 'named' itself
    q.push(named);
    report("push(lvalue)     ");

    // emplace forwards the arguments and constructs the element inside the queue:
    // one construction, no temporary, nothing to move.
    q.emplace("logs", 3);
    report("emplace(args...) ");

    std::cout << "queued: " << q.size() << ", first topic = " << q.front().topic << '\n';
    return 0;
}
