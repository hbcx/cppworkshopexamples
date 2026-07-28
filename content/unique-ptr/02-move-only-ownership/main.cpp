#include <iostream>
#include <memory>
#include <utility>

// unique_ptr models UNIQUE ownership: exactly one owner at a time, so it cannot
// be copied. You transfer ownership with std::move, which leaves the source
// empty (null) but still valid.
class Widget {
public:
    explicit Widget(int id) : id_(id) {}
    int id() const { return id_; }
private:
    int id_;
};

// A sink: it takes the unique_ptr by value, so calling it moves ownership in
// and the Widget is destroyed when this function returns.
static void consume(std::unique_ptr<Widget> w) {
    std::cout << "consume got Widget " << w->id() << '\n';
}

int main() {
    std::unique_ptr<Widget> a(new Widget(42));

    // std::unique_ptr<Widget> copy = a;    // would NOT compile: copy is deleted.

    std::unique_ptr<Widget> b = std::move(a);   // ownership moves a -> b
    std::cout << "after move: a holds " << (a ? "yes" : "no")
              << ", b holds " << (b ? "yes" : "no")
              << " (id " << b->id() << ")\n";

    consume(std::move(b));                       // ownership moves b -> consume
    std::cout << "after sink: b holds " << (b ? "yes" : "no") << '\n';

    // A moved-from unique_ptr is empty, not broken: give it a new object.
    b.reset(new Widget(7));
    std::cout << "b reused, now holds id " << b->id() << '\n';
    return 0;
}
