#ifndef PIMPL_INLINE_GADGET_HPP
#define PIMPL_INLINE_GADGET_HPP

#include <memory>
#include <string>

// WRONG (does not compile) -- do NOT do this:
//     class Gadget {
//     public:
//         explicit Gadget(std::string id);
//         // no destructor declared, OR:  ~Gadget() = default;   // in the header
//         ...
//     };
// Either way the compiler generates ~Gadget() HERE, where Impl is incomplete.
// Destroying unique_ptr<Impl> needs a complete Impl, so the build fails with
// "deleting incomplete type" / "invalid application of sizeof". The same holds
// for a move constructor or move assignment defaulted in the header.
//
// RIGHT: DECLARE the special members here, DEFINE them in the .cpp where Impl
// is a complete type.
class Gadget {
public:
    explicit Gadget(std::string id);
    ~Gadget();                          // declared here, defined in .cpp
    Gadget(Gadget&&) noexcept;          // declared here, defined in .cpp
    Gadget& operator=(Gadget&&) noexcept;
    std::string id() const;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

#endif
