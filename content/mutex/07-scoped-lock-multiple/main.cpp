#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

int main() {
    // Locking TWO mutexes is where deadlocks come from: if one thread locks A then B
    // while another locks B then A, each can grab one and then wait forever for the
    // other. std::scoped_lock (C++17) locks any number of mutexes AT ONCE with a
    // deadlock-avoidance algorithm, so the order you name them does not matter and
    // no thread can get stuck. Like lock_guard it is RAII -- it unlocks them all in
    // its destructor.
    std::mutex left, right;
    long from = 4000, to = 0;         // repeatedly move one unit from 'from' to 'to'

    std::vector<std::thread> pool;
    for (int i = 0; i < 4; ++i)
        pool.emplace_back([&, i] {
            for (int k = 0; k < 1000; ++k) {
                if (i % 2 == 0) {
                    std::scoped_lock guard(left, right);    // names left, right
                    --from; ++to;
                } else {
                    std::scoped_lock guard(right, left);    // opposite order: still safe
                    --from; ++to;
                }
            }
        });
    for (auto& t : pool) t.join();

    // Each move keeps from + to constant, and holding BOTH mutexes every time makes
    // the pair of writes mutually exclusive. With hand-written left.lock()/
    // right.lock() in opposite orders, this program could deadlock instead.
    std::cout << "from + to = " << (from + to) << '\n';   // 4000
    return 0;
}
