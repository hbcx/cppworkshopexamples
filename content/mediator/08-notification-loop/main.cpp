// Anti-pattern: an infinite notification loop.
//
// When a mediator reacts to a colleague changing by changing another colleague,
// and that change notifies the mediator too, the reactions can feed back on each
// other forever. Here two currency fields are kept in sync: setting USD makes the
// mediator set EUR, whose change makes it set USD, and so on. Without a guard the
// update never settles -- the loop below is bounded only by a hop cap that stops
// the hang so you can see it. The fix is to stop propagating when nothing
// actually changed, so the feedback dies out after one round.

#include <iostream>

class Field;

class Mediator {
public:
    virtual ~Mediator() = default;
    virtual void changed(Field* f) = 0;
};

int g_updates = 0;
const int CAP = 12; // safety cap so the buggy version cannot truly hang

class Field {
    int value_ = 0;
    Mediator* mediator_;
    bool guard_; // GOOD fields skip the notify when the value did not change
public:
    Field(Mediator* m, bool guard) : mediator_(m), guard_(guard) {}
    int value() const { return value_; }
    void set(int v) {
        if (guard_ && v == value_) return; // fix: no change -> no notification
        value_ = v;
        if (++g_updates > CAP) { std::cout << "  ...aborted (runaway loop)\n"; return; }
        mediator_->changed(this);
    }
};

// Keeps USD and EUR in sync at a 10:9 rate.
class Converter : public Mediator {
public:
    Field usd, eur;
    Converter(bool guard) : usd(this, guard), eur(this, guard) {}
    void changed(Field* f) override {
        if (f == &usd) eur.set(usd.value() * 9 / 10);
        else           usd.set(eur.value() * 10 / 9);
    }
};

int main() {
    std::cout << "-- bad (no guard): set USD = 10 --\n";
    g_updates = 0;
    Converter bad(false);
    bad.usd.set(10); // ping-pongs until the cap trips
    std::cout << "  updates: " << g_updates << "\n";

    std::cout << "-- good (guard): set USD = 10 --\n";
    g_updates = 0;
    Converter good(true);
    good.usd.set(10); // settles: USD=10 -> EUR=9 -> USD=10 (unchanged) -> stop
    std::cout << "  USD=" << good.usd.value() << " EUR=" << good.eur.value()
              << " in " << g_updates << " updates\n";
    return 0;
}
