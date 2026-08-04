// Anti-pattern: a shallow snapshot of an owned resource.
//
// When the state owns a heap resource, the memento must copy the DATA, not just
// the handle. A memento that copies a shared_ptr to the buffer shares the same
// buffer with the originator, so an in-place edit after saving mutates the
// "snapshot" too, and restore hands back the modified data. The fix is a deep
// copy: the memento holds its own copy of the resource, taken at save time.

#include <iostream>
#include <vector>
#include <memory>
#include <utility>

// ---- BAD: the memento shares the buffer (copies the handle, not the data) ----
class CanvasBad {
    std::shared_ptr<std::vector<int>> pixels_;
public:
    CanvasBad() : pixels_(std::make_shared<std::vector<int>>()) {}
    void push(int p) { pixels_->push_back(p); } // mutates the buffer in place
    int sum() const { int s = 0; for (int p : *pixels_) s += p; return s; }

    class Memento {
        std::shared_ptr<std::vector<int>> shared_; // BUG: same buffer as the canvas
    public:
        explicit Memento(std::shared_ptr<std::vector<int>> p) : shared_(std::move(p)) {}
        std::shared_ptr<std::vector<int>> handle() const { return shared_; }
    };
    Memento save() const { return Memento(pixels_); }         // shares, does not copy
    void restore(const Memento& m) { pixels_ = m.handle(); }
};

// ---- GOOD: the memento deep-copies the resource ----
class CanvasGood {
    std::vector<int> pixels_;
public:
    void push(int p) { pixels_.push_back(p); }
    int sum() const { int s = 0; for (int p : pixels_) s += p; return s; }

    class Memento {
        std::vector<int> copy_; // an independent copy of the data
        explicit Memento(std::vector<int> c) : copy_(std::move(c)) {}
        friend class CanvasGood;
    };
    Memento save() const { return Memento(pixels_); }         // deep copy now
    void restore(const Memento& m) { pixels_ = m.copy_; }
};

int main() {
    CanvasBad bad;
    bad.push(1);
    CanvasBad::Memento snapBad = bad.save(); // "snapshot" at sum 1
    bad.push(2);                             // edits the shared buffer
    bad.restore(snapBad);                    // restore: same buffer, now [1,2]
    std::cout << "bad restored sum:  " << bad.sum() << "\n"; // 3 -- snapshot was shared

    CanvasGood good;
    good.push(1);
    CanvasGood::Memento snapGood = good.save(); // independent copy at sum 1
    good.push(2);
    good.restore(snapGood);
    std::cout << "good restored sum: " << good.sum() << "\n"; // 1
    return 0;
}
