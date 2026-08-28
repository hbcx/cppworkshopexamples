#include <iostream>
#include <string>

// A Widget plays two independent roles. Each role is a pure interface, and Widget
// implements both -- the clean use of multiple inheritance.

struct Drawable {
    virtual void draw() const = 0;
    virtual ~Drawable() = default;
};

struct Serializable {
    virtual std::string serialize() const = 0;
    virtual ~Serializable() = default;
};

class Widget : public Drawable, public Serializable {
public:
    explicit Widget(std::string label) : label_(std::move(label)) {}
    void draw() const override { std::cout << "[ " << label_ << " ]\n"; }
    std::string serialize() const override { return "Widget{" + label_ + "}"; }
private:
    std::string label_;
};

// Each function needs only one of the roles:
void render(const Drawable& d) { d.draw(); }
void save(const Serializable& s) { std::cout << "saved: " << s.serialize() << "\n"; }

int main() {
    Widget w{"OK"};

    render(w);   // [ OK ]        -- used as a Drawable
    save(w);     // saved: Widget{OK}  -- the same object, used as a Serializable
    return 0;
}
