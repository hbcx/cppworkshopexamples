#include <algorithm>
#include <iostream>

class AudioDevice {
public:
    virtual ~AudioDevice() = default;

    // Public, non-virtual: clamps to the valid range BEFORE the virtual step, so
    // no subclass can ever receive or apply an out-of-range volume. The
    // invariant lives in one place and cannot be skipped.
    void setVolume(int v) {
        int clamped = std::max(0, std::min(100, v));
        std::cout << "request " << v << " -> clamped " << clamped << ": ";
        applyVolume(clamped);
    }

private:
    virtual void applyVolume(int v) = 0;
};

class Speaker : public AudioDevice {
    void applyVolume(int v) override { std::cout << "speaker at " << v << "\n"; }
};

int main() {
    Speaker s;
    s.setVolume(50);      // in range
    s.setVolume(150);     // clamped to 100
    s.setVolume(-20);     // clamped to 0

    // The override never sees 150 or -20; the non-virtual wrapper guarantees
    // 0..100. In a public-virtual design each override would have to clamp, and
    // one that forgot would apply a bad value.
    return 0;
}
