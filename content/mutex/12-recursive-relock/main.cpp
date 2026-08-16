#include <iostream>
#include <mutex>

std::mutex m;
int balance = 100;

// ANTI-PATTERN (described, not run): a method that holds the lock calls ANOTHER
// method that locks the SAME non-recursive mutex. The second lock() blocks on a
// mutex this very thread already holds -- a self-deadlock (locking a std::mutex you
// already own is undefined behaviour).
//
//   void withdraw(int x){ std::lock_guard g(m); balance -= x; log_balance(); }
//   void log_balance(){ std::lock_guard g(m); std::cout << balance; }  // re-locks m!
//
// It hides easily: log_balance() is correct on its own, and only deadlocks when
// called from something that already holds the lock.

// FIX: split the work. The PUBLIC method locks once; the shared logic lives in a
// private helper that assumes the lock is already held and never locks itself.
namespace detail {
    void log_locked() {                        // precondition: caller holds m
        std::cout << "balance = " << balance << '\n';
    }
}
void withdraw(int x) {
    std::lock_guard<std::mutex> g(m);
    balance -= x;
    detail::log_locked();                      // no second lock -> no self-deadlock
}

int main() {
    withdraw(30);                              // balance 70
    withdraw(20);                              // balance 50

    // std::recursive_mutex would ALSO let the same thread lock again, so the naive
    // version would "work" with it -- but it is usually a design smell: it hides
    // that you hold a lock across a call that re-enters, and it makes reasoning
    // about your invariants harder. Prefer the locked-helper split above; reach for
    // recursive_mutex only when a genuinely re-entrant API leaves no alternative.
    return 0;
}
