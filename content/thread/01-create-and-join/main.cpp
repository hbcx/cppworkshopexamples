#include <iostream>
#include <string>
#include <thread>

int main() {
    // A std::thread starts running its callable IMMEDIATELY on construction, on a
    // new OS thread, in parallel with main. Here the worker fills in a result; main
    // then waits for it with join() before reading what it produced.
    std::string result;
    std::thread worker([&result] {
        result = "done by the worker thread";
    });

    // join() blocks until the thread finishes. After it returns, the worker is done
    // and its writes are guaranteed visible here -- join creates a happens-before
    // edge -- so reading result is race-free. (Only main prints, and only after the
    // join, so there is no interleaving of output between the two threads.)
    worker.join();

    std::cout << result << '\n';

    // The rule that governs every std::thread: it MUST be joined or detached before
    // it is destroyed. join is the usual choice -- it ties the worker's lifetime to
    // this scope. Skipping it is the section's first anti-pattern.
    return 0;
}
