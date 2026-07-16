#include <iostream>
#include <queue>
#include <string>
#include <utility>

struct Job {
    std::string name;
    explicit Job(std::string n) : name(std::move(n)) {}
};

// The safe primitive: take the element OUT, then remove it. There is no window
// in which a caller can hold a reference into a popped slot.
static Job take(std::queue<Job>& q) {
    Job job = std::move(q.front());   // move out first...
    q.pop();                          // ...then destroy the husk
    return job;
}

int main() {
    std::queue<Job> jobs;
    jobs.push(Job("render"));
    jobs.push(Job("encode"));
    jobs.push(Job("upload"));

    // Anti-pattern (shown, NOT run): front() returns a REFERENCE, and pop()
    // destroys the object it refers to:
    //   const Job& next = jobs.front();
    //   jobs.pop();                     // 'next' now refers to a destroyed element
    //   std::cout << next.name;         // UB: reads a dead object
    // It usually "works" in a debug run -- the freed bytes are still readable --
    // and starts failing under load or with a different allocator.

    // Fix 1: copy the value out before popping.
    Job copied = jobs.front();          // a value, not a reference
    jobs.pop();
    std::cout << "copied out: " << copied.name << '\n';

    // Fix 2: move it out before popping. Same order, no copy. Note that 'auto'
    // deduces a VALUE here, while 'auto&' or 'const auto&' would keep the
    // reference -- the habit that is right everywhere else is wrong before a pop.
    auto moved = std::move(jobs.front());
    jobs.pop();
    std::cout << "moved out:  " << moved.name << '\n';

    // Fix 3: hide the order in a helper, so it cannot be got wrong.
    Job taken = take(jobs);
    std::cout << "taken:      " << taken.name << '\n';

    std::cout << "all three still valid: " << copied.name << ' ' << moved.name
              << ' ' << taken.name << '\n';
    std::cout << "queue is empty? " << (jobs.empty() ? "yes" : "no") << '\n';
    return 0;
}
