#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

// Component.
struct Graphic {
    virtual void draw() const = 0;
    virtual void translate(int dx, int dy) = 0;
    virtual ~Graphic() = default;
};

// Leaf.
class Dot : public Graphic {
public:
    Dot(std::string name, int x, int y) : name_(std::move(name)), x_(x), y_(y) {}
    void draw() const override { std::cout << name_ << " at (" << x_ << "," << y_ << ")\n"; }
    void translate(int dx, int dy) override { x_ += dx; y_ += dy; }
private:
    std::string name_;
    int x_;
    int y_;
};

// Composite: a group applies an operation to itself by applying it to every
// child. translate() on a group moves the entire subtree, and draw() renders all
// of it -- the client moves a whole scene with one call and never distinguishes a
// single shape from a group of them.
class Group : public Graphic {
public:
    void add(std::unique_ptr<Graphic> g) { items_.push_back(std::move(g)); }
    void draw() const override {
        for (const auto& g : items_) g->draw();
    }
    void translate(int dx, int dy) override {
        for (const auto& g : items_) g->translate(dx, dy);
    }
private:
    std::vector<std::unique_ptr<Graphic>> items_;
};

int main() {
    auto scene = std::make_unique<Group>();
    scene->add(std::make_unique<Dot>("A", 0, 0));

    auto pair = std::make_unique<Group>();      // a group nested inside a group
    pair->add(std::make_unique<Dot>("B", 10, 10));
    pair->add(std::make_unique<Dot>("C", 20, 20));
    scene->add(std::move(pair));

    std::cout << "before:\n";
    scene->draw();

    scene->translate(5, 5);      // one call moves every shape in the whole tree

    std::cout << "after translate(5,5):\n";
    scene->draw();
    return 0;
}
