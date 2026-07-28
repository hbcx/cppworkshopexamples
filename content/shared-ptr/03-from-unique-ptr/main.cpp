#include <iostream>
#include <memory>
#include <utility>

class Widget {
public:
    explicit Widget(int id) : id_(id) { std::cout << "Widget " << id_ << " born\n"; }
    ~Widget() { std::cout << "Widget " << id_ << " gone\n"; }
    int id() const { return id_; }
private:
    int id_;
};

// A factory returns unique_ptr: single ownership by default. The caller is free
// to keep it unique OR widen it to shared -- their choice, not the factory's.
static std::unique_ptr<Widget> makeWidget(int id) {
    return std::unique_ptr<Widget>(new Widget(id));
}

int main() {
    std::unique_ptr<Widget> u = makeWidget(1);

    // unique_ptr -> shared_ptr is a supported one-line move: ownership transfers,
    // the shared_ptr adopts the unique_ptr's deleter, and u is left empty.
    std::shared_ptr<Widget> s = std::move(u);
    std::cout << "converted: u holds " << (u ? "yes" : "no")
              << ", s holds Widget " << s->id()
              << " (use_count " << s.use_count() << ")\n";

    std::shared_ptr<Widget> s2 = s;   // now genuinely shared
    std::cout << "shared use_count = " << s.use_count() << '\n';

    // The reverse does NOT exist. There is no shared_ptr -> unique_ptr conversion
    // and no shared_ptr::release(), because a shared_ptr may have other owners
    // and so can never return EXCLUSIVE ownership. That is why factories hand
    // back unique_ptr: callers can always widen to shared, never the other way.
    return 0;
}
