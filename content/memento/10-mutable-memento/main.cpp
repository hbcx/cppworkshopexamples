// Anti-pattern: a mutable memento.
//
// A snapshot should be immutable once taken: it records a value at a moment, and
// nothing should be able to change that record afterward. A mutable memento --
// public, assignable state -- lets a stored snapshot be edited after capture, so
// an unrelated bug or a careless reuse corrupts a value the undo history depends
// on, and a later restore reinstates the corrupted value. The fix is to make the
// captured state unmodifiable after construction (private, only the originator a
// friend, no setters), so a stored memento cannot be altered.

#include <iostream>

// ---- BAD: the memento's captured value can be changed after saving ----
class SettingsBad {
    int volume_;
public:
    explicit SettingsBad(int v) : volume_(v) {}
    struct Memento { int volume; }; // public, assignable
    Memento save() const { return Memento{ volume_ }; }
    void restore(const Memento& m) { volume_ = m.volume; }
    int volume() const { return volume_; }
};

// ---- GOOD: the captured value is sealed once constructed ----
class SettingsGood {
    int volume_;
public:
    explicit SettingsGood(int v) : volume_(v) {}
    class Memento {
        int volume_;
        explicit Memento(int v) : volume_(v) {}
        friend class SettingsGood; // only the originator reads it; no one can write it
    };
    Memento save() const { return Memento(volume_); }
    void restore(const Memento& m) { volume_ = m.volume_; }
    int volume() const { return volume_; }
};

int main() {
    SettingsBad bad(50);
    SettingsBad::Memento snapBad = bad.save(); // captured 50
    bad = SettingsBad(80);                     // volume changes to 80 in the meantime
    snapBad.volume = 999;                      // a stray write corrupts the stored snapshot
    bad.restore(snapBad);
    std::cout << "bad restored:  " << bad.volume() << "\n"; // 999, not the captured 50

    SettingsGood good(50);
    SettingsGood::Memento snapGood = good.save(); // captured 50
    good = SettingsGood(80);
    // snapGood.volume_ = 999;   // would NOT compile: private, no setter
    good.restore(snapGood);
    std::cout << "good restored: " << good.volume() << "\n"; // 50, sealed
    return 0;
}
