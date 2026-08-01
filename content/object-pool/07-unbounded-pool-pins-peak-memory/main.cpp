#include <iostream>
#include <memory>
#include <vector>

struct Slot {
    int id;
    explicit Slot(int i) : id(i) {}
};

// Anti-pattern: release() always keeps the object on the free list. After a
// one-time burst the pool holds the peak count forever -- a high-water mark
// pinned in memory even when demand has dropped back to near zero.
class UnboundedPool {
    struct Return { UnboundedPool* pool; void operator()(Slot* s) const { pool->release(s); } };
public:
    using Handle = std::unique_ptr<Slot, Return>;
    Handle acquire() {
        Slot* s;
        if (!free_.empty()) { s = free_.back().release(); free_.pop_back(); }
        else { s = new Slot(nextId_++); }
        return Handle(s, Return{this});
    }
    std::size_t retained() const { return free_.size(); }
private:
    void release(Slot* s) { free_.push_back(std::unique_ptr<Slot>(s)); }   // never lets go
    std::vector<std::unique_ptr<Slot>> free_;
    int nextId_ = 1;
};

// Fix: bound the pool. Past the cap, a returned object is destroyed instead of
// hoarded, so idle memory falls back toward the cap after a burst passes.
class BoundedPool {
    struct Return { BoundedPool* pool; void operator()(Slot* s) const { pool->release(s); } };
public:
    using Handle = std::unique_ptr<Slot, Return>;
    explicit BoundedPool(std::size_t cap) : cap_(cap) {}
    Handle acquire() {
        Slot* s;
        if (!free_.empty()) { s = free_.back().release(); free_.pop_back(); }
        else { s = new Slot(nextId_++); }
        return Handle(s, Return{this});
    }
    std::size_t retained() const { return free_.size(); }
private:
    void release(Slot* s) {
        if (free_.size() < cap_) free_.push_back(std::unique_ptr<Slot>(s));
        else delete s;                 // over capacity: let it go
    }
    std::vector<std::unique_ptr<Slot>> free_;
    std::size_t cap_;
    int nextId_ = 1;
};

int main() {
    // A burst of five slots in use at once, then all returned.
    {
        UnboundedPool pool;
        {
            std::vector<UnboundedPool::Handle> burst;
            for (int i = 0; i < 5; ++i) burst.push_back(pool.acquire());
        }   // all five returned here
        std::cout << "unbounded  -> retained after burst = " << pool.retained() << '\n';
    }

    // Same burst through a pool capped at two: it keeps two, frees the rest.
    {
        BoundedPool pool(2);
        {
            std::vector<BoundedPool::Handle> burst;
            for (int i = 0; i < 5; ++i) burst.push_back(pool.acquire());
        }
        std::cout << "bounded(2) -> retained after burst = " << pool.retained() << '\n';
    }
    return 0;
}
