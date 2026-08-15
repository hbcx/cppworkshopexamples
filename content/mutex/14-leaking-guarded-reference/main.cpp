#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

class Registry {
public:
    // ANTI-PATTERN (described): returning a reference INTO the guarded data. The
    // lock_guard inside get_ref() is destroyed when get_ref() returns, so the caller
    // then reads and writes data_ with NO lock held -- a race, even though the
    // accessor itself locked.
    //   std::string& get_ref() { std::lock_guard<std::mutex> g(m_); return data_; }

    // FIX 1: return a COPY made under the lock. The caller owns an independent value
    // and needs no lock to use it.
    std::string snapshot() const {
        std::lock_guard<std::mutex> g(m_);
        return data_;                          // copied while the lock is held
    }
    // FIX 2: bring the work TO the data. Pass a function that runs while the lock is
    // held, so the guarded data never leaves the critical section.
    template <class F>
    void with_locked(F&& f) {
        std::lock_guard<std::mutex> g(m_);
        f(data_);
    }
private:
    mutable std::mutex m_;
    std::string data_ = "start";               // 5 characters
};

int main() {
    Registry r;
    std::vector<std::thread> pool;
    for (int i = 0; i < 4; ++i)
        pool.emplace_back([&, i] {
            for (int k = 0; k < 1000; ++k)
                r.with_locked([i](std::string& s) { s += static_cast<char>('A' + i % 3); });
        });
    for (auto& t : pool) t.join();

    std::cout << "length = " << r.snapshot().size() << '\n';   // 5 + 4000 = 4005

    // The rule: never hand a caller a reference or pointer into data your mutex
    // protects. Once it escapes the critical section the lock no longer covers it.
    // Return a copy, or run the caller's code inside the lock.
    return 0;
}
