// Anti-pattern: a snapshot that aliases the live state.
//
// A memento must COPY the state it captures. If it instead stores a reference or
// pointer to the originator's live field, it is not a snapshot at all -- it
// tracks the field as it changes, so by the time you restore, it hands back the
// CURRENT value, not the one you saved. The rollback becomes a no-op. The fix is
// to copy the value into the memento at save time.

#include <iostream>
#include <string>
#include <utility>

// ---- BAD: the memento holds a pointer to the live member ----
class EditorBad {
    std::string text_;
public:
    class Memento {
        const std::string* live_; // BUG: points at text_, does not copy it
    public:
        explicit Memento(const std::string* p) : live_(p) {}
        const std::string& value() const { return *live_; }
    };
    void type(const std::string& s) { text_ += s; }
    const std::string& text() const { return text_; }
    Memento save() const { return Memento(&text_); }       // aliases the live field
    void restore(const Memento& m) { text_ = m.value(); }  // reads the CURRENT text
};

// ---- GOOD: the memento holds a copy taken at save time ----
class EditorGood {
    std::string text_;
public:
    class Memento {
        std::string text_;
        explicit Memento(std::string t) : text_(std::move(t)) {}
        friend class EditorGood;
    };
    void type(const std::string& s) { text_ += s; }
    const std::string& text() const { return text_; }
    Memento save() const { return Memento(text_); }        // copies now
    void restore(const Memento& m) { text_ = m.text_; }
};

int main() {
    EditorBad bad;
    bad.type("Hello");
    EditorBad::Memento snapBad = bad.save(); // "snapshot" at "Hello"
    bad.type(", world");                     // mutate after saving
    bad.restore(snapBad);                    // restore reads the live, mutated text
    std::cout << "bad restored:  " << bad.text() << "\n"; // "Hello, world" -- no rollback

    EditorGood good;
    good.type("Hello");
    EditorGood::Memento snapGood = good.save(); // real copy of "Hello"
    good.type(", world");
    good.restore(snapGood);
    std::cout << "good restored: " << good.text() << "\n"; // "Hello"
    return 0;
}
