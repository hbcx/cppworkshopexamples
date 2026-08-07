// The reason RAII matters: a function has many exits -- the end, an early
// return, a thrown exception -- and manual cleanup has to be repeated on every
// one of them. Miss a single path and the resource leaks. A destructor runs on
// ALL of those paths for free, so RAII cleans up once and never misses.
//
// We model a resource with a live counter: acquire() adds one, release() takes
// one away. At the end the count should be back to zero; a leak shows as count
// still above zero.

#include <iostream>
#include <stdexcept>

int live = 0;

int  acquire(const char* who) { ++live; std::cout << "acquire " << who << " (live=" << live << ")\n"; return live; }
void release(const char* who) { --live; std::cout << "release " << who << " (live=" << live << ")\n"; }

void doWork() { throw std::runtime_error("boom"); }   // an operation that fails

// Manual cleanup: the release is written AFTER the work, so when doWork throws
// the release is jumped over and the resource leaks.
void manual() {
    acquire("manual");
    doWork();               // throws -> the next line is skipped
    release("manual");      // only runs on the no-throw path
}

// RAII cleanup: the guard's destructor releases the resource while the
// exception unwinds the stack, so it is freed no matter how the block is left.
struct Guard {
    const char* who;
    explicit Guard(const char* w) : who(w) { acquire(who); }
    ~Guard() { release(who); }
};

void raii() {
    Guard g("raii");
    doWork();               // throws -> g's destructor still runs on the way out
}

int main() {
    try { manual(); } catch (...) { std::cout << "manual() threw\n"; }
    std::cout << "after manual: live=" << live << "  <- leaked\n\n";

    live = 0;   // the leaked resource above is gone for good; isolate the RAII demo
    try { raii(); } catch (...) { std::cout << "raii() threw\n"; }
    std::cout << "after raii:   live=" << live << "  <- clean\n";
}
