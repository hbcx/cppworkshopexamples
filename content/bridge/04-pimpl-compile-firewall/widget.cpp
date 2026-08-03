#include "widget.hpp"

#include <iostream>
#include <utility>

// The implementation is COMPLETELY hidden in this translation unit. Add fields,
// change algorithms, pull in heavy headers -- none of it touches widget.hpp or
// the files that include it.
struct Widget::Impl {
    std::string name;
    int greetings = 0;
};

Widget::Widget(std::string name) : impl_(std::make_unique<Impl>()) {
    impl_->name = std::move(name);
}

// These MUST be defined here, not defaulted in the header: destroying a
// unique_ptr<Impl> requires Impl to be a complete type, which it is only here.
Widget::~Widget() = default;
Widget::Widget(Widget&&) noexcept = default;
Widget& Widget::operator=(Widget&&) noexcept = default;

void Widget::greet() {
    ++impl_->greetings;
    std::cout << "hello from " << impl_->name << " (greeting #" << impl_->greetings << ")\n";
}
