#ifndef WIDGET_HPP
#define WIDGET_HPP

#include <memory>
#include <string>

// pimpl (pointer to implementation) is a degenerate Bridge: the public class is
// the abstraction, the hidden Impl is its sole implementation, and the pointer
// between them is the bridge. Its purpose is not runtime variation but a COMPILE
// firewall -- this header names no implementation detail, so changing Impl
// recompiles only widget.cpp, not every file that includes this header, and the
// class layout the header exposes never changes (a stable ABI).
class Widget {
public:
    explicit Widget(std::string name);
    ~Widget();                             // declared here, DEFINED in the .cpp
    Widget(Widget&&) noexcept;             // where Impl is a complete type, so
    Widget& operator=(Widget&&) noexcept;  // unique_ptr<Impl> can be destroyed
    void greet();
private:
    struct Impl;                           // forward declaration only
    std::unique_ptr<Impl> impl_;
};

#endif
