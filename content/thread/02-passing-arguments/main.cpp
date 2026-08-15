#include <atomic>
#include <functional>
#include <iostream>
#include <thread>

void fill(int factor, int& out) {   // out is a reference parameter
    out = factor * 10;
}

int main() {
    // Any arguments you list after the callable are forwarded to it. The catch that
    // surprises everyone: std::thread first COPIES each argument into its own
    // internal storage, then calls the function with those copies. So a reference
    // parameter does NOT bind to your variable -- it binds to the thread's private
    // copy, and your variable is never touched.
    int value = 0;

    //   std::thread bad(fill, 5, value);   // compiles, but fill writes to a COPY;
    //                                       // 'value' would stay 0 after join.

    // To pass a real reference, wrap the argument in std::ref. That preserves the
    // reference through the thread's copying, so fill writes back to 'value'.
    std::thread t(fill, 5, std::ref(value));
    t.join();
    std::cout << "value via std::ref = " << value << '\n';   // 50

    // Plain by-value arguments are just copied, which is normally what you want. Two
    // safety notes: the copy means a pointer or reference you pass must outlive the
    // thread, and std::ref opts OUT of that copy -- the referenced object must stay
    // alive for the whole run (fine here, we join in the same scope).
    std::atomic<int> parallel_sum{0};
    auto add = [&parallel_sum](int n) { parallel_sum.fetch_add(n); };  // n copied in
    std::thread a(add, 20);
    std::thread b(add, 22);
    a.join();
    b.join();
    std::cout << "sum of copied args = " << parallel_sum.load() << '\n';   // 42
    return 0;
}
