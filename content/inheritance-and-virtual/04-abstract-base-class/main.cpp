#include <iostream>

// Drawable is an interface: it declares draw() as pure virtual and provides no
// body. It cannot be instantiated; only concrete widgets can.

class Drawable {
public:
    virtual void draw() const = 0;          // pure virtual -> Drawable is abstract
    virtual ~Drawable() = default;
};

class Button : public Drawable {
public:
    explicit Button(const char* label) : label_(label) {}
    void draw() const override { std::cout << "[ " << label_ << " ]\n"; }
private:
    const char* label_;
};

class Separator : public Drawable {
public:
    void draw() const override { std::cout << "--------\n"; }
};

// Depends only on the interface, not on any concrete widget.
void render(const Drawable& d) { d.draw(); }

int main() {
    Button ok{"OK"};
    Separator sep;

    render(ok);    // [ OK ]
    render(sep);   // --------

    // Drawable d;  // would NOT compile: cannot instantiate an abstract class
    return 0;
}
