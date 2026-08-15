#include <iostream>
#include <system_error>
#include <thread>

int main() {
    std::cout << std::boolalpha;

    // ANTI-PATTERN: calling join() (or detach()) on a std::thread that is no longer
    // joinable -- because it was already joined, already detached, moved-from, or
    // default-constructed. It does not quietly do nothing: it throws
    // std::system_error. Guard with joinable(), or structure the code so each thread
    // is finished exactly once.

    // Case 1: joining twice. The first join consumes the thread; the second throws.
    {
        std::thread t([] {});
        t.join();                       // ok: waits, leaves t non-joinable
        try {
            t.join();                   // t is no longer joinable -> throws
        } catch (const std::system_error& e) {
            // Print a stable fact, not the message text (which varies by library).
            std::cout << "double join threw system_error: "
                      << (e.code() == std::errc::invalid_argument) << '\n';   // true
        }
    }

    // Case 2: joining after detach. detach() already released the thread, so it is
    // not joinable -- the joinable() guard is what keeps this from throwing.
    {
        std::thread t([] {});
        t.detach();
        std::cout << "after detach, joinable? " << t.joinable() << '\n';   // false
        if (t.joinable())
            t.join();
        else
            std::cout << "skipped join because thread is not joinable\n";
    }

    // FIX: check joinable() before joining, and keep ownership clear so you never
    // try to finish the same thread twice. std::jthread removes the manual join
    // entirely, so this whole class of mistake disappears.
    return 0;
}
