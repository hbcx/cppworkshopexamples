// Anti-pattern: an unbounded memento history.
//
// A caretaker that keeps every snapshot forever grows without limit. For full
// snapshots of real state this is a memory leak in slow motion: a long editing
// session accumulates a copy of the document for every edit, and the undo
// history can dwarf the document. The fix is a bounded history that keeps only
// the last N snapshots (dropping the oldest), trading unlimited undo depth for
// bounded memory -- or storing deltas instead of full snapshots.

#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <cstddef>

// ---- BAD: history grows one snapshot per edit, forever ----
class UnboundedHistory {
    std::vector<std::string> snaps_;
public:
    void save(const std::string& state) { snaps_.push_back(state); }
    std::size_t size() const { return snaps_.size(); }
};

// ---- GOOD: keep only the most recent N snapshots ----
class BoundedHistory {
    std::deque<std::string> snaps_;
    std::size_t cap_;
public:
    explicit BoundedHistory(std::size_t cap) : cap_(cap) {}
    void save(const std::string& state) {
        snaps_.push_back(state);
        if (snaps_.size() > cap_)
            snaps_.pop_front(); // drop the oldest
    }
    std::size_t size() const { return snaps_.size(); }
};

int main() {
    UnboundedHistory unbounded;
    BoundedHistory bounded(10);

    for (int i = 0; i < 1000; ++i) {
        std::string state = "state-" + std::to_string(i);
        unbounded.save(state);
        bounded.save(state);
    }

    std::cout << "after 1000 edits:\n";
    std::cout << "  unbounded history holds: " << unbounded.size() << " snapshots\n";
    std::cout << "  bounded history holds:   " << bounded.size() << " snapshots\n";
    return 0;
}
