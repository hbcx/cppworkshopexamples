#include <iostream>
#include <mutex>
#include <stdexcept>

std::mutex m;
int shared = 0;

// ANTI-PATTERN (described, not run): a hand-managed lock with an exit path that
// skips the unlock. The mutex stays locked, and the next thread to lock it -- or
// even the next call on this thread -- blocks forever.
//
//   void deposit(int x) {
//       m.lock();
//       if (x < 0) return;       // <-- early return: m is NEVER unlocked -> deadlock
//       shared += x;
//       m.unlock();
//   }
//
// The same happens if anything between lock() and unlock() throws: the unlock is
// skipped by the stack unwinding.

// FIX: std::lock_guard unlocks in its destructor on EVERY exit path -- an early
// return, a thrown exception, or normal fall-through -- so the mutex is always
// released. You cannot forget it.
void deposit(int x) {
    std::lock_guard<std::mutex> guard(m);
    if (x < 0) return;                                   // guard unlocks here
    if (x > 1000) throw std::runtime_error("too big");   // and here (during unwinding)
    shared += x;
}                                                        // and here

int main() {
    deposit(10);
    deposit(-5);                       // early return -- but the lock was released
    try { deposit(2000); } catch (const std::exception&) {}   // threw -- lock released
    deposit(32);                       // still works: proof the mutex was freed each time
    std::cout << "shared = " << shared << '\n';   // 42
    return 0;
}
