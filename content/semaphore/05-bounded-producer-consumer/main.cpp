#include <iostream>
#include <semaphore>
#include <thread>

// The classic use of counting semaphores: a bounded (fixed-size) queue with
// backpressure. Two semaphores track the buffer's state -- empty_slots starts at
// the capacity, full_slots at zero. The producer acquires an empty slot before it
// writes and releases a full one after; the consumer does the mirror image.
// Neither can run away: a full buffer blocks the producer, an empty one blocks the
// consumer. No mutex is needed because each slot is handed off exactly once.
int main() {
    const int CAP = 4;
    const int COUNT = 20;                    // items to move through the buffer
    int buffer[CAP] = {};
    std::counting_semaphore<CAP> empty_slots{CAP};   // CAP writable slots
    std::counting_semaphore<CAP> full_slots{0};      // 0 readable slots

    long total = 0;
    std::jthread producer([&] {
        for (int i = 1; i <= COUNT; ++i) {
            empty_slots.acquire();           // wait for room
            buffer[i % CAP] = i;             // write into the slot
            full_slots.release();            // announce one item is ready
        }
    });
    std::jthread consumer([&] {
        for (int i = 1; i <= COUNT; ++i) {
            full_slots.acquire();            // wait for an item
            total += buffer[i % CAP];        // read the slot
            empty_slots.release();           // free the slot for reuse
        }
    });
    // jthread auto-joins in its destructor -- but that runs at the end of main,
    // AFTER the line below reads total. We join explicitly here so both threads
    // have finished (and their writes to total are visible) before we read it.
    producer.join();
    consumer.join();

    std::cout << "moved " << COUNT << " items, sum = " << total << '\n';   // 210
    return 0;
}
