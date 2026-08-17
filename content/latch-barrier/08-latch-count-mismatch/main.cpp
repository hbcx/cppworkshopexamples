#include <iostream>
#include <latch>
#include <thread>
#include <vector>

// Anti-pattern: fewer count_down() calls than the latch's count.
//
// A latch(N) only reaches zero after N count_downs. If a worker takes an early
// exit -- an error return, a thrown exception, a branch that forgets to signal --
// one count_down is missing and wait() blocks FOREVER:
//
//   std::latch done(N);
//   worker: if (failed) return;        // <-- forgot to count_down on this path
//           done.count_down();
//   done.wait();                       // hangs: the count never reaches zero
//   // We do not run that deadlocking version.
//
// The fix is to make count_down happen on EVERY path, including early returns and
// exceptions, with an RAII guard whose destructor counts down.
struct CountDownGuard {
    std::latch& latch;
    ~CountDownGuard() { latch.count_down(); }   // fires on any exit from the scope
};

int main() {
    const int N = 4;
    std::latch done(N);

    std::vector<std::jthread> workers;
    for (int i = 0; i < N; ++i) {
        workers.emplace_back([&done, i] {
            CountDownGuard guard{done};      // counts down no matter how we leave
            if (i % 2 == 0) {
                return;                      // "early failure" -- still counts down
            }
            // ... normal work path ...
        });
    }

    done.wait();                        // completes: all 4 counted down, come what may
    std::cout << "latch released despite early exits = " << std::boolalpha
              << true << '\n';                  // true (we got past wait)
    return 0;
}
