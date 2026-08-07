// Anti-pattern: a scope guard with no name.
//
// A guard only protects the code that runs BEFORE its destructor. A named local
// is destroyed at the end of its block, so it covers the whole block. But a
// guard built as an unnamed TEMPORARY is destroyed at the end of the full
// expression -- the semicolon on that line -- so it is already gone before the
// next statement runs. The classic form is a lock:
//
//     std::lock_guard<std::mutex>{m};   // BUG: locks then unlocks right away
//     critical_section();               // runs with NO lock held
//
// It compiles, and on a single-threaded test it even "works", because nothing
// races. The protection is simply absent. We model the lock with a "held" flag
// so the mistake is visible without threads: inside a real critical section
// held should be true.

#include <iostream>

bool held = false;

struct Lock {
    Lock()  { held = true;  std::cout << "  lock   (held=" << held << ")\n"; }
    ~Lock() { held = false; std::cout << "  unlock (held=" << held << ")\n"; }
};

void bad() {
    std::cout << "bad:\n";
    Lock{};                    // unnamed temporary -> destroyed at this semicolon
    std::cout << "  critical work, held=" << held << "  <- already false!\n";
}

void good() {
    std::cout << "good:\n";
    Lock lock;                 // named local -> lives to the end of the function
    std::cout << "  critical work, held=" << held << "  <- protected\n";
}

int main() {
    bad();
    good();
}
