#include <chrono>
#include <iostream>
#include <semaphore>

using namespace std::chrono_literals;

// acquire() blocks until a permit is free -- possibly forever. When you would
// rather give up after a while, try_acquire_for() waits at most a duration and
// returns whether it got a permit. There is also try_acquire() (no wait at all) and
// try_acquire_until() (wait until a time point). Use them to add a timeout or a
// fallback instead of blocking with no way out.
int main() {
    std::counting_semaphore<2> slots{2};    // start with 2 permits

    // Permits are available: try_acquire_for returns true right away, no waiting.
    bool first = slots.try_acquire_for(50ms);
    bool second = slots.try_acquire_for(50ms);
    std::cout << "got two permits: " << std::boolalpha << first << ' ' << second << '\n';  // true true

    // Now both are taken and nobody will release. A third try waits up to the
    // timeout, then gives up and returns false -- the caller can do something else.
    bool third = slots.try_acquire_for(20ms);
    std::cout << "third within timeout: " << third << '\n';   // false

    slots.release();                        // tidy up the two we took
    slots.release();
    return 0;
}
