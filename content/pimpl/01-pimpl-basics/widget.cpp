#include "widget.hpp"

#include <iostream>

// Impl is fully defined HERE, where it is a complete type.
struct Widget::Impl {
    std::string name;
};

Widget::Widget(std::string name) : impl_(std::make_unique<Impl>()) {
    impl_->name = std::move(name);
}

// Defined where Impl is complete, so unique_ptr's deleter can see sizeof(Impl).
Widget::~Widget() = default;

void Widget::greet() const {
    std::cout << "Hello from " << impl_->name << "\n";
}

void Widget::rename(const std::string& name) {
    impl_->name = name;
}
