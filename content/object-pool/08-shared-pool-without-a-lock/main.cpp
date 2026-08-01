#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

struct Slot {
    int id;
    explicit Slot(int i) : id(i) {}
};

// A pool with a single free slot -- the contended case. acquire() is really two
// steps: look at a free slot, then take it off the list. Between the two steps
// the slot is still on the list, which is the whole problem under concurrency.
class Pool {
public:
    Pool() {
        owned_.push_back(std::make_unique<Slot>(1));
        free_.push_back(owned_.back().get());
    }

    // UNSYNCHRONIZED, split into its two real steps so the race window is
    // visible: two threads can both peek() before either take()s.
    Slot* peek() const { return free_.empty() ? nullptr : free_.back(); }
    void  take() { if (!free_.empty()) free_.pop_back(); }

    // SYNCHRONIZED: a lock makes look-and-take one atomic step, so a slot is
    // handed to exactly one caller.
    Slot* acquireLocked() {
        std::lock_guard<std::mutex> lock(mtx_);
        if (free_.empty()) return nullptr;
        Slot* s = free_.back();
        free_.pop_back();
        return s;
    }
    void releaseLocked(Slot* s) {
        std::lock_guard<std::mutex> lock(mtx_);
        free_.push_back(s);
    }

private:
    std::vector<std::unique_ptr<Slot>> owned_;
    std::vector<Slot*> free_;
    std::mutex mtx_;
};

int main() {
    // The bug, reproduced deterministically by interleaving the two steps the
    // way two unsynchronized threads would: both peek the one free slot before
    // either takes it. (A real data race is undefined behaviour, so we do NOT
    // run one -- we replay the exact interleaving single-threaded.)
    {
        Pool pool;
        Slot* a = pool.peek();   // thread A sees slot #1
        Slot* b = pool.peek();   // thread B sees slot #1 -- still on the list!
        pool.take();             // one real removal; the second thread's take
                                 // would corrupt the list, but the damage is
                                 // already done: A and B share one slot.
        std::cout << "unsynchronized -> A got #" << a->id << ", B got #" << b->id
                  << (a == b ? "  (same slot handed out twice)\n" : "\n");
    }

    // The fix: acquireLocked() makes look-and-take atomic, so the single free
    // slot goes to exactly one caller; the second correctly gets nothing.
    {
        Pool pool;
        Slot* a = pool.acquireLocked();
        Slot* b = pool.acquireLocked();
        std::cout << "synchronized   -> A got #" << a->id
                  << ", B got " << (b ? "#" + std::to_string(b->id) : std::string("nothing"))
                  << (b == nullptr ? "  (slot issued once)\n" : "\n");
        if (a) pool.releaseLocked(a);
    }
    return 0;
}
