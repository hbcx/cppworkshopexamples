#include <iostream>
#include <semaphore>

// Anti-pattern: releasing more permits than the semaphore's maximum.
//
// std::counting_semaphore<LeastMaxValue> can hold at most max() permits, and max()
// is at least the LeastMaxValue you pass as the template argument. release(n) has a
// PRECONDITION: the resulting count must not exceed max(). Break it and the program
// has undefined behaviour -- no exception, no clamp, just UB:
//
//   std::counting_semaphore<2> slots{2};   // full: 2 of 2 permits
//   slots.release();                       // count 2 -> 3 > max: UB. We do NOT run this.
//
// This shows up when release() is called without a matching acquire() -- for
// instance a "producer" that releases a full-slot semaphore once per item but is
// not throttled by an empty-slot semaphore, so its count climbs past the buffer
// size.
//
// The fix is to size the maximum to the real capacity and only release a permit you
// conceptually took, keeping the count inside [0, max]. Below, a correctly sized
// semaphore is filled to exactly its max and drained, never over-releasing.
int main() {
    const int CAP = 3;
    std::counting_semaphore<3> slots{0};    // room for up to 3 permits, starts empty

    for (int i = 0; i < CAP; ++i) {
        slots.release();                    // 0 -> 1 -> 2 -> 3: never exceeds max
    }

    int taken = 0;
    while (slots.try_acquire()) ++taken;    // drain every permit back out

    std::cout << "released and drained exactly " << taken << " permits\n";  // 3
    return 0;
}
