#include <future>
#include <iostream>

// Anti-pattern: assuming std::async(fn) with no policy runs on a thread.
//
// The default launch policy is std::launch::async | std::launch::deferred --
// the implementation may pick EITHER. If it defers, the task does not run on a
// background thread, and it does not run AT ALL until someone calls get() or
// wait(). So relying on the default to give you concurrency, or to run a task
// for its side effects, is a bug: the work may silently never happen.
//
// We cannot force a given implementation to defer, so we show the hazard with
// launch::deferred explicitly -- which is one of the two things the default is
// allowed to choose -- and a task whose future is never waited on.

int main() {
    bool side_effect_ran = false;

    // Deferred (a legal choice of the default) + future never used: the task
    // never runs. This is the silent-loss case the default policy can produce.
    {
        std::future<void> f = std::async(std::launch::deferred,
                                         [&] { side_effect_ran = true; });
        // ... we forget to call f.get()/f.wait(); f goes out of scope here ...
    }
    std::cout << "deferred, never waited on: side effect ran = "
              << std::boolalpha << side_effect_ran << '\n';     // false

    // The fix: if you need the work to actually run on a thread, ask for it.
    side_effect_ran = false;
    std::future<void> f = std::async(std::launch::async,
                                     [&] { side_effect_ran = true; });
    f.get();                             // and always consume the result
    std::cout << "explicit launch::async + get: side effect ran = "
              << side_effect_ran << '\n';                       // true
    return 0;
}
