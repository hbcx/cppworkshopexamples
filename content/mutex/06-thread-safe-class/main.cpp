#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

// A thread-safe counter: the data and the mutex that guards it live TOGETHER, and
// every method locks before touching the data. Callers never see the raw value, so
// they cannot forget to lock -- the safety is built into the type. This is the
// classic "monitor" pattern: encapsulate the shared state behind a locked interface.
class Counter {
public:
    void increment() {
        std::lock_guard<std::mutex> guard(m_);
        ++value_;
    }
    long get() const {
        std::lock_guard<std::mutex> guard(m_);   // reads lock too, to see a stable value
        return value_;                            // return a COPY, not a reference in
    }
private:
    mutable std::mutex m_;   // 'mutable' so a const method (get) may lock it
    long value_ = 0;
};

int main() {
    Counter c;
    std::vector<std::thread> pool;
    for (int i = 0; i < 4; ++i)
        pool.emplace_back([&] {
            for (int k = 0; k < 100000; ++k) c.increment();
        });
    for (auto& t : pool) t.join();

    std::cout << "counter = " << c.get() << '\n';   // 400000

    // Two design points this bakes in: the mutex is 'mutable' so const read methods
    // can still lock, and get() returns a COPY -- handing back a reference to value_
    // would let a caller read it with no lock held (an anti-pattern later). One
    // side effect: a std::mutex member is non-copyable, so Counter is non-copyable
    // too, which is usually what you want for a shared object.
    return 0;
}
