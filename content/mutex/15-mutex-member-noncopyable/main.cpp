#include <iostream>
#include <mutex>

// std::mutex is NON-copyable and NON-movable: there is one underlying lock, and
// duplicating it makes no sense. So a class with a std::mutex member has its copy
// and move operations implicitly DELETED -- `Config b = a;` would not compile if
// you left it at the default.
class Config {
public:
    Config() = default;

    // ANTI-PATTERN (described): relying on the default copy, which is deleted here.
    //   Config b = a;               // error: use of deleted copy constructor
    //
    // FIX: if the type genuinely needs to be copyable, write copy operations by
    // hand. Lock the source, copy the DATA (never the mutex), and let the copy get
    // its own fresh, default-constructed mutex.
    Config(const Config& other) {
        std::lock_guard<std::mutex> g(other.m_);
        value_ = other.value_;                 // m_ is default-constructed, not copied
    }
    Config& operator=(const Config& other) {
        if (this != &other) {
            // Lock both without risking deadlock: std::lock orders them safely, and
            // adopt_lock hands ownership to the guards so they still unlock via RAII.
            std::lock(m_, other.m_);
            std::lock_guard<std::mutex> g1(m_, std::adopt_lock);
            std::lock_guard<std::mutex> g2(other.m_, std::adopt_lock);
            value_ = other.value_;
        }
        return *this;
    }

    void set(int v) { std::lock_guard<std::mutex> g(m_); value_ = v; }
    int  get() const { std::lock_guard<std::mutex> g(m_); return value_; }
private:
    mutable std::mutex m_;
    int value_ = 0;
};

int main() {
    Config a;
    a.set(7);
    Config b = a;                 // uses our hand-written, lock-taking copy ctor
    b.set(9);
    std::cout << "a = " << a.get() << ", b = " << b.get() << '\n';   // 7, 9

    // The point: a std::mutex member makes a type non-copyable and non-movable by
    // default. Often that is exactly right for a shared object. When you do need
    // copying, define it yourself and copy only the data under a lock -- do not try
    // to copy or move the mutex.
    return 0;
}
