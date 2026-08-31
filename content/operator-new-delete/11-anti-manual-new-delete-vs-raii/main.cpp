#include <iostream>
#include <memory>
#include <stdexcept>

struct Resource {
    static int alive;
    int id;
    explicit Resource(int i) : id(i) { ++alive; }
    ~Resource() { --alive; }
};
int Resource::alive = 0;

void doWork(bool fail) {
    if (fail) throw std::runtime_error("work failed");
}

// BAD: manual new/delete. When doWork throws, control leaves the function and
// `delete a` is skipped -- the Resource leaks.
void manual() {
    Resource* a = new Resource(1);
    doWork(true);      // throws -> the next line never runs
    delete a;          // skipped on the throwing path
}

// GOOD: RAII owns the resource. The unique_ptr destructor runs while the
// exception unwinds the stack, so nothing leaks -- no try/catch needed.
void raii() {
    auto a = std::make_unique<Resource>(1);
    doWork(true);      // throws -> a is destroyed automatically
}

int main() {
    try { manual(); } catch (const std::exception&) {}
    std::cout << "after manual(): alive = " << Resource::alive
              << " (leaked)\n";

    Resource::alive = 0;
    try { raii(); } catch (const std::exception&) {}
    std::cout << "after raii():   alive = " << Resource::alive
              << " (clean)\n";
}
