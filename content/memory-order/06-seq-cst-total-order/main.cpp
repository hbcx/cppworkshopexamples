#include <atomic>
#include <iostream>
#include <thread>

// seq_cst does one thing release/acquire cannot: it places ALL seq_cst operations into
// a single global total order that every thread agrees on. The "store buffering" shape
// below needs exactly that. Each thread stores 1 to its own flag and then reads the
// OTHER flag. Under seq_cst the two stores sit in one global order, so at least one
// thread runs its store before its load -- both threads reading 0 is impossible. With
// only release/acquire (or relaxed) there is no global order across the two independent
// variables, and on weakly-ordered hardware BOTH loads can return 0. We do not rely on
// reproducing that (it is rare on x86); we run the seq_cst version and confirm its
// guarantee holds every round.
int main() {
    bool both_zero_ever = false;
    const int ROUNDS = 1000;

    for (int r = 0; r < ROUNDS; ++r) {
        std::atomic<int> x{0}, y{0};
        int read_y = -1, read_x = -1;

        std::jthread a([&] { x.store(1); read_y = y.load(); });   // seq_cst (default)
        std::jthread b([&] { y.store(1); read_x = x.load(); });
        a.join();
        b.join();

        if (read_x == 0 && read_y == 0) both_zero_ever = true;    // forbidden by seq_cst
    }

    std::cout << "seq_cst forbids both-zero, ever seen = " << std::boolalpha
              << both_zero_ever << '\n';   // false
    return 0;
}
