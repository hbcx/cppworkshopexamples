// Anti-pattern: a gap between acquiring the resource and the guard taking it.
//
// RAII is only safe if acquisition and ownership are the SAME step. If you
// acquire a raw resource, run some code, and only THEN construct the guard, any
// exception thrown in that gap leaks the resource -- the guard was never built,
// so no destructor will release it.
//
//     int fd = open_file();    // acquired, owned by nobody yet
//     validate();              // if this throws, fd leaks
//     Guard g(fd);             // takes ownership -- too late
//
// The fix: acquire INSIDE the guard's constructor, so the resource is owned the
// instant it exists.

#include <iostream>
#include <stdexcept>

int live = 0;
int  acquire() { ++live; std::cout << "acquire (live=" << live << ")\n"; return live; }
void release(int) { --live; std::cout << "release (live=" << live << ")\n"; }

void validate() { throw std::runtime_error("bad input"); }

// Takes an ALREADY-open handle. There is a gap before it is constructed.
struct Guard {
    int h;
    explicit Guard(int handle) : h(handle) {}
    ~Guard() { release(h); }
    Guard(const Guard&) = delete;
    Guard& operator=(const Guard&) = delete;
};

// BROKEN: acquire, then a throwing step, THEN the guard. The throw skips the
// guard's construction, so nothing ever releases the handle.
void bad() {
    int h = acquire();     // owned by nobody
    validate();            // throws -> h leaks
    Guard g(h);            // never reached
    (void)g;
}

// Acquires in its OWN constructor: obtaining and owning are one step.
struct SafeGuard {
    int h;
    SafeGuard() : h(acquire()) {}
    ~SafeGuard() { release(h); }
    SafeGuard(const SafeGuard&) = delete;
    SafeGuard& operator=(const SafeGuard&) = delete;
};

// FIXED: the guard owns the handle from the instant it exists, so a later throw
// unwinds through it and releases the resource.
void good() {
    SafeGuard g;           // owned from here on
    validate();            // throws -> g's destructor releases during unwinding
    (void)g;
}

int main() {
    try { bad(); }  catch (...) { std::cout << "bad() threw\n"; }
    std::cout << "after bad:  live=" << live << "  <- leaked\n\n";

    live = 0;   // the leaked handle above is gone for good; isolate the fixed demo
    try { good(); } catch (...) { std::cout << "good() threw\n"; }
    std::cout << "after good: live=" << live << "  <- clean\n";
}
