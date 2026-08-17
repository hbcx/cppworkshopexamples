#include <iostream>
#include <semaphore>
#include <string>
#include <thread>

// Two binary semaphores can make two threads take strict turns. Each thread waits
// on its own semaphore, does its step, then releases the OTHER thread's -- so they
// alternate A, B, A, B, ... in a fixed order. This is a hand-off in both
// directions, which a single mutex cannot express: a mutex has no way to say "your
// turn next". Only the thread holding its permit ever touches the shared log, so
// there is no race even without a lock around it.
int main() {
    const int ROUNDS = 3;
    std::binary_semaphore a_turn{1};    // A goes first
    std::binary_semaphore b_turn{0};
    std::string log;

    std::jthread a([&] {
        for (int i = 0; i < ROUNDS; ++i) {
            a_turn.acquire();           // wait for my turn
            log += 'A';                 // safe: only A runs between its acquire/release
            b_turn.release();           // hand the turn to B
        }
    });
    std::jthread b([&] {
        for (int i = 0; i < ROUNDS; ++i) {
            b_turn.acquire();
            log += 'B';
            a_turn.release();           // hand the turn back to A
        }
    });
    a.join();
    b.join();

    std::cout << "turn order: " << log << '\n';   // ABABAB
    return 0;
}
