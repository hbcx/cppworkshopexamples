#ifndef PIMPL_BASICS_WIDGET_HPP
#define PIMPL_BASICS_WIDGET_HPP

#include <memory>
#include <string>

// The public class exposes NO implementation detail: Impl is only
// forward-declared, and the header includes nothing the internals need.
class Widget {
public:
    explicit Widget(std::string name);
    ~Widget();                        // MUST be declared here, defined in the .cpp
    void greet() const;
    void rename(const std::string& name);

private:
    struct Impl;                      // forward declaration -- incomplete here
    std::unique_ptr<Impl> impl_;
};

#endif
