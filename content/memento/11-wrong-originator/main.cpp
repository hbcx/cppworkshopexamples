// Anti-pattern: restoring a memento into the wrong originator.
//
// A memento belongs to the object it was taken from. When mementos are type-
// erased into a shared snapshot type (so a generic history can hold them), that
// belonging is lost, and a snapshot from one originator can be handed to another
// -- restoring state that means nothing there, silently corrupting it. The
// strongest defence is the type system (a nested Originator::Memento cannot be
// applied to a different type at all); when you must type-erase, tag each
// snapshot with its originator and reject a mismatch at restore.

#include <iostream>

// A type-erased snapshot, as a generic undo system might store.
struct Snapshot {
    int originatorId;
    int state;
};

// ---- BAD: restore accepts any snapshot, from any originator ----
class ThingBad {
    int id_;
    int state_;
public:
    ThingBad(int id, int s) : id_(id), state_(s) {}
    Snapshot save() const { return Snapshot{ id_, state_ }; }
    void restore(const Snapshot& s) { state_ = s.state; } // no check
    int state() const { return state_; }
};

// ---- GOOD: reject a snapshot that did not come from this originator ----
class ThingGood {
    int id_;
    int state_;
public:
    ThingGood(int id, int s) : id_(id), state_(s) {}
    Snapshot save() const { return Snapshot{ id_, state_ }; }
    void restore(const Snapshot& s) {
        if (s.originatorId != id_) {
            std::cout << "  rejected: snapshot from originator " << s.originatorId
                      << " does not belong to " << id_ << "\n";
            return;
        }
        state_ = s.state;
    }
    int state() const { return state_; }
};

int main() {
    ThingBad a(1, 100), b(2, 7);
    Snapshot fromA = a.save(); // a snapshot of thing 1
    b.restore(fromA);          // wrongly applied to thing 2
    std::cout << "bad: thing2 state = " << b.state() << "\n"; // 100 -- corrupted

    ThingGood c(1, 100), d(2, 7);
    Snapshot fromC = c.save();
    d.restore(fromC);          // rejected by the id check
    std::cout << "good: thing2 state = " << d.state() << "\n"; // 7 -- unchanged
    return 0;
}
