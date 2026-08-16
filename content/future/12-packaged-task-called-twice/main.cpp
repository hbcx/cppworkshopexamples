#include <future>
#include <iostream>

// Anti-pattern: invoking the same std::packaged_task more than once.
//
// A packaged_task fulfils its future exactly once. The first call runs the
// callable and stores the result; a second call has nowhere to put a result --
// the future is already satisfied -- so it throws std::future_error with
// future_errc::promise_already_satisfied. A packaged_task is a single-use job,
// not a reusable function object. To run the work again, build a new task.
long square(long n) { return n * n; }

int main() {
    std::packaged_task<long(long)> task(square);
    std::future<long> f = task.get_future();

    task(5);                             // first call: fulfils the future
    std::cout << "first call, result = " << f.get() << '\n';     // 25

    // The trap: calling it again throws (defined behaviour, so we run + catch).
    try {
        task(6);
        std::cout << "second call succeeded\n";                  // never reached
    } catch (const std::future_error& e) {
        bool already = (e.code() == std::future_errc::promise_already_satisfied);
        std::cout << "second call threw, already_satisfied = "
                  << std::boolalpha << already << '\n';           // true
    }

    // The fix: one task per run. Make a fresh task (with its own future) each
    // time you need to execute the work again.
    std::packaged_task<long(long)> again(square);
    std::future<long> f2 = again.get_future();
    again(6);
    std::cout << "fresh task, result = " << f2.get() << '\n';     // 36
    return 0;
}
