#include <future>
#include <iostream>
#include <thread>

// std::promise and std::future are the two ends of a one-shot channel. The
// promise is the WRITE end: one thread calls set_value once. The future is the
// READ end: another thread calls get() and blocks until the value arrives.
// Unlike std::async, here YOU decide which thread produces the value and when --
// the producer does not have to be the thread that started the consumer. Use
// this when async's "call a function, get its return" shape does not fit and you
// need to hand a value across a boundary yourself.
void produce(std::promise<int> p) {
    // ... compute something, maybe after waiting for an event ...
    int answer = 6 * 7;
    p.set_value(answer);            // fulfils the promise; unblocks the future
}

int main() {
    std::promise<int> p;
    std::future<int> f = p.get_future();   // grab the read end BEFORE moving p

    // Hand the write end to another thread. promise is move-only.
    std::thread producer(produce, std::move(p));

    int value = f.get();            // blocks here until produce() sets the value
    std::cout << "future received value = " << value << '\n';   // 42

    producer.join();
    return 0;
}
