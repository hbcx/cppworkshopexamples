// Anti-pattern: a snapshot that misses part of the state.
//
// A memento must capture ALL the state needed to restore the originator. If it
// saves some fields and forgets others, restore reinstates the saved fields but
// leaves the forgotten ones at their current values -- a partial rollback that
// silently mixes old and new state. Here a shape saves its position but not its
// colour, so restoring after a move-and-recolour brings back the old position
// with the new colour. The fix is to capture every field restore must reset.

#include <iostream>
#include <string>
#include <utility>

// ---- BAD: the memento saves position but forgets colour ----
class ShapeBad {
    int x_, y_;
    std::string color_;
public:
    ShapeBad(int x, int y, std::string c) : x_(x), y_(y), color_(std::move(c)) {}
    class Memento {
        int x_, y_; // colour is missing
        Memento(int x, int y) : x_(x), y_(y) {}
        friend class ShapeBad;
    };
    Memento save() const { return Memento(x_, y_); }
    void restore(const Memento& m) { x_ = m.x_; y_ = m.y_; } // colour_ left as-is
    void moveTo(int x, int y) { x_ = x; y_ = y; }
    void recolor(const std::string& c) { color_ = c; }
    void print() const { std::cout << "(" << x_ << "," << y_ << ") " << color_ << "\n"; }
};

// ---- GOOD: the memento captures every field restore must reset ----
class ShapeGood {
    int x_, y_;
    std::string color_;
public:
    ShapeGood(int x, int y, std::string c) : x_(x), y_(y), color_(std::move(c)) {}
    class Memento {
        int x_, y_;
        std::string color_;
        Memento(int x, int y, std::string c) : x_(x), y_(y), color_(std::move(c)) {}
        friend class ShapeGood;
    };
    Memento save() const { return Memento(x_, y_, color_); }
    void restore(const Memento& m) { x_ = m.x_; y_ = m.y_; color_ = m.color_; }
    void moveTo(int x, int y) { x_ = x; y_ = y; }
    void recolor(const std::string& c) { color_ = c; }
    void print() const { std::cout << "(" << x_ << "," << y_ << ") " << color_ << "\n"; }
};

int main() {
    ShapeBad bad(10, 10, "red");
    ShapeBad::Memento snapBad = bad.save();
    bad.moveTo(20, 20);
    bad.recolor("blue");
    bad.restore(snapBad);
    std::cout << "bad restored:  "; bad.print(); // (10,10) blue -- colour not rolled back

    ShapeGood good(10, 10, "red");
    ShapeGood::Memento snapGood = good.save();
    good.moveTo(20, 20);
    good.recolor("blue");
    good.restore(snapGood);
    std::cout << "good restored: "; good.print(); // (10,10) red
    return 0;
}
