#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

// The same idea as example 01, but now the wrapper is a regular COPYABLE
// value. The Concept gains a clone() so the wrapper can duplicate the
// object it holds without knowing its type.
class Shape {
public:
    template <class T>
    Shape(T x) : self_(new Model<T>(std::move(x))) {}

    // Copy deep-copies the held object through clone().
    Shape(const Shape& other) : self_(other.self_->clone()) {}
    // Copy-and-swap: one operator= serves both copy and move assignment.
    Shape& operator=(Shape other) { self_ = std::move(other.self_); return *this; }
    Shape(Shape&&) noexcept = default;

    std::string name() const { return self_->name(); }

private:
    struct Concept {
        virtual ~Concept() = default;
        virtual std::string name() const = 0;
        virtual std::unique_ptr<Concept> clone() const = 0;
    };
    template <class T>
    struct Model : Concept {
        Model(T x) : data_(std::move(x)) {}
        std::string name() const override { return data_.name(); }
        std::unique_ptr<Concept> clone() const override {
            return std::unique_ptr<Concept>(new Model<T>(data_));
        }
        T data_;
    };
    std::unique_ptr<Concept> self_;
};

struct Dog  { std::string name() const { return "Dog"; } };
struct Tree { std::string name() const { return "Tree"; } };

int main() {
    std::vector<Shape> shapes;
    shapes.push_back(Dog{});
    shapes.push_back(Tree{});

    // Copy the whole vector: each held object is deep-copied via clone(),
    // so the copy is fully independent of the original.
    std::vector<Shape> copy = shapes;
    copy.push_back(Dog{});

    std::cout << "original: " << shapes.size() << "\n";
    for (const Shape& s : shapes) std::cout << "  " << s.name() << "\n";
    std::cout << "copy: " << copy.size() << "\n";
    for (const Shape& s : copy) std::cout << "  " << s.name() << "\n";
    return 0;
}
