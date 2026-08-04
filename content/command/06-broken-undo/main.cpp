#include <iostream>

class Register {
public:
    void set(int v) { value_ = v; }
    int value() const { return value_; }
private:
    int value_ = 0;
};

// ANTI-PATTERN: undo() does not restore the PRIOR value -- it resets to a
// hardcoded default (0). That looks right when the undone action followed a set
// from zero, but after a chain of sets, undo throws away the real previous value
// and corrupts the state. An undo must reverse to what was there before, not to a
// guess.
class SetValueBad {
public:
    SetValueBad(Register& r, int v) : reg_(r), v_(v) {}
    void execute() { reg_.set(v_); }
    void undo() { reg_.set(0); }        // BUG: guesses the prior state
private:
    Register& reg_;
    int v_;
};

// FIX: capture the prior value on execute(), and restore exactly it on undo().
// This is Command meeting Memento -- the command stores a snapshot of what it is
// about to overwrite so it can put it back.
class SetValueGood {
public:
    SetValueGood(Register& r, int v) : reg_(r), v_(v) {}
    void execute() {
        prev_ = reg_.value();           // remember what was there
        reg_.set(v_);
    }
    void undo() { reg_.set(prev_); }    // restore it exactly
private:
    Register& reg_;
    int v_;
    int prev_ = 0;
};

int main() {
    std::cout << "== broken undo (resets to 0) ==\n";
    Register r;
    SetValueBad a(r, 10);
    SetValueBad b(r, 20);
    a.execute();                        // 10
    b.execute();                        // 20
    b.undo();                           // should restore 10...
    std::cout << "after undo: " << r.value() << " (wanted 10)\n";    // 0 -- wrong

    std::cout << "== correct undo (restores the prior value) ==\n";
    Register r2;
    SetValueGood c(r2, 10);
    SetValueGood d(r2, 20);
    c.execute();                        // 10
    d.execute();                        // 20
    d.undo();                           // restores 10
    std::cout << "after undo: " << r2.value() << " (wanted 10)\n";   // 10 -- correct
    return 0;
}
