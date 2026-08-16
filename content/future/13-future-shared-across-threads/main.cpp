#include <future>
#include <iostream>
#include <thread>
#include <vector>

// Anti-pattern: letting several threads call get() on the same std::future.
//
// A std::future is a single-consumer, move-only handle. get() is a non-const,
// consuming operation, and the future is not safe to call get()/wait() on from
// more than one thread at once. Two threads sharing one std::future by reference
// is a data race -- undefined behaviour -- and only one of them could take the
// value anyway. When many threads must observe one result, use a
// std::shared_future and give each thread its OWN copy.
//
//   // THE BUG (undefined behaviour -- described, not run):
//   std::future<long> f = std::async(std::launch::async, produce);
//   auto racer = [&f]{ long v = f.get(); use(v); };   // same future, many threads
//   std::thread a(racer), b(racer);                   // data race on f, and only
//   a.join(); b.join();                               // one get() can succeed

long produce() { return 100; }

int main() {
    // The fix: one shared_future, copied to each waiter. Copies are independent
    // handles to the same result; get() is safe from every copy and returns the
    // same value to all.
    std::shared_future<long> result =
        std::async(std::launch::async, produce).share();

    const int N = 4;
    std::vector<long> seen(N, 0);
    std::vector<std::thread> workers;
    for (int i = 0; i < N; ++i)
        workers.emplace_back([result, &seen, i] {   // capture OWN copy by value
            seen[i] = result.get();                  // safe from every thread
        });
    for (auto& w : workers) w.join();

    bool all_same = true;
    for (long v : seen) all_same = all_same && (v == 100);
    std::cout << "shared_future, all " << N << " threads saw 100 = "
              << std::boolalpha << all_same << '\n';             // true
    return 0;
}
