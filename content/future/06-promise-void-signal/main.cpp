#include <future>
#include <iostream>
#include <thread>

// A std::promise<void> carries no value -- it is a pure one-shot signal. The
// waiter blocks on the future until the promise is fulfilled with set_value().
// Because the future establishes a happens-before, anything the signalling
// thread wrote BEFORE set_value() is safely visible to the waiter AFTER it
// wakes, even though the promise itself transports nothing. This is a clean
// "wait until I say go" latch between two threads, with no mutex of your own.
int shared_config = 0;                 // written by main before the go signal

int main() {
    std::promise<void> go;
    std::future<void> go_signal = go.get_future();

    std::thread worker([&go_signal] {
        go_signal.wait();              // block until main fulfils the promise
        // Safe to read shared_config now: main set it before set_value(), and
        // that write happens-before this wait returns.
        std::cout << "worker started, config = " << shared_config << '\n';  // 99
    });

    // ... do setup that must finish before the worker is allowed to run ...
    shared_config = 99;
    go.set_value();                    // release the worker (no data, just "go")

    worker.join();
    return 0;
}
