#include <iostream>

int frameDepth = 0;   // how deep we are inside the wrapper's fixed frame

// The NVI base: draw() wraps the virtual paint() in a fixed frame.
struct Canvas {
    virtual ~Canvas() = default;
    void draw() {
        ++frameDepth;
        std::cout << "[frame open, depth=" << frameDepth << "]\n";
        paint();
        std::cout << "[frame close, depth=" << frameDepth << "]\n";
        --frameDepth;
    }
private:
    virtual void paint() = 0;
};

// BAD: the override calls the PUBLIC wrapper draw() again instead of doing its
// own painting, so the fixed frame runs a second time -- nested. Without the
// depth guard this would recurse until the stack overflows.
struct BadShape : Canvas {
    void paint() override {
        if (frameDepth < 2) draw();          // re-enters the wrapper (guarded so it stops)
        else std::cout << "  actual paint\n";
    }
};

// GOOD: the override does only its own work and never calls back into the
// public wrapper, so the frame opens and closes exactly once.
struct GoodShape : Canvas {
    void paint() override { std::cout << "  actual paint\n"; }
};

int main() {
    std::cout << "bad -- override re-enters draw():\n";
    BadShape{}.draw();

    std::cout << "\ngood -- override paints only:\n";
    GoodShape{}.draw();
    return 0;
}
