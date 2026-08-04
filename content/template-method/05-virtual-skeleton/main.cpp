// Anti-pattern: a virtual (overridable) skeleton.
//
// The whole value of Template Method is that the base OWNS the algorithm's shape
// and a subclass can only fill in steps. Make the template method itself virtual
// and that guarantee evaporates: a subclass can override the skeleton wholesale,
// dropping mandatory steps and reordering the rest, so the invariant the base
// promised no longer holds. Here ReportBad::render() is virtual and a subclass
// replaces it, skipping the header and footer every report is supposed to have.
// The fix is to keep the skeleton non-virtual and expose only the steps.

#include <iostream>

// ---------- BAD: the skeleton is virtual, so a subclass can discard it ----------
class ReportBad {
public:
    virtual ~ReportBad() = default;
    virtual void render() {          // virtual skeleton: overridable in full
        header();
        body();
        footer();
    }
protected:
    virtual void body() { std::cout << "  (empty)\n"; }
    void header() { std::cout << "== BEGIN ==\n"; }
    void footer() { std::cout << "== END ==\n"; }
};

class RogueReport : public ReportBad {
public:
    void render() override {         // replaces the whole algorithm...
        body();                      // ...and skips header() and footer()
    }
protected:
    void body() override { std::cout << "  rogue body, no header/footer\n"; }
};

// ---------- GOOD: non-virtual skeleton, only the steps vary ----------
class ReportGood {
public:
    virtual ~ReportGood() = default;
    void render() {                  // non-virtual: the shape is the invariant
        header();
        body();
        footer();
    }
protected:
    virtual void body() = 0;         // the only extension point
private:
    void header() { std::cout << "== BEGIN ==\n"; }
    void footer() { std::cout << "== END ==\n"; }
};

class SalesReport : public ReportGood {
protected:
    void body() override { std::cout << "  sales body\n"; }
};

int main() {
    std::cout << "--- bad (skeleton overridden, header/footer lost) ---\n";
    RogueReport{}.render();

    std::cout << "--- good (skeleton fixed, steps vary) ---\n";
    SalesReport{}.render();
    return 0;
}
