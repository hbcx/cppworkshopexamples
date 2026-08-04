// Anti-pattern: leaking the state to the client.
//
// State works because the object owns its transitions: the client asks for
// something meaningful (power on, play) and the object decides whether that is
// legal and what state it lands in. Expose the raw state -- a public field the
// client reads and writes, or a setState the client calls directly -- and that
// guarantee is gone: the client can force an illegal state the object would
// never allow, and starts branching on the concrete state itself, dragging the
// state logic back out into every caller. The fix is to keep the state private
// and offer only intent-level operations the object guards.

#include <iostream>

// ---------- BAD: the state is public; the client drives it directly ----------
class PlayerBad {
public:
    enum class S { Off, Playing };
    S state = S::Off;              // public: any caller can read and write it
    void setState(S s) { state = s; } // and set it directly, past any rule
};

// ---------- GOOD: state private; only guarded, intent-level operations ----------
class PlayerGood {
    bool on_ = false;
    bool playing_ = false;
public:
    void powerOn() { on_ = true; std::cout << "power on\n"; }
    void play() {
        if (!on_) { std::cout << "play denied: power is off\n"; return; } // the object guards it
        playing_ = true;
        std::cout << "playing\n";
    }
    bool isPlaying() const { return playing_; } // ask about behaviour, not raw state
};

int main() {
    std::cout << "--- bad ---\n";
    PlayerBad bad;
    bad.setState(PlayerBad::S::Playing); // forced straight into Playing, power still Off
    // The client also has to branch on the raw state itself:
    if (bad.state == PlayerBad::S::Playing)
        std::cout << "playing while power is off: inconsistent\n";

    std::cout << "--- good ---\n";
    PlayerGood good;
    good.play();     // refused: power is off
    good.powerOn();  // the legal path first
    good.play();     // now allowed
    std::cout << "isPlaying=" << (good.isPlaying() ? "yes" : "no") << "\n";
    return 0;
}
