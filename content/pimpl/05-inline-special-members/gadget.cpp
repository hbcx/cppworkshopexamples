#include "gadget.hpp"

struct Gadget::Impl {
    std::string id;
};

Gadget::Gadget(std::string id) : impl_(std::make_unique<Impl>()) {
    impl_->id = std::move(id);
}

// All defined here, where Impl is complete -- this is what the header cannot do.
Gadget::~Gadget() = default;
Gadget::Gadget(Gadget&&) noexcept = default;
Gadget& Gadget::operator=(Gadget&&) noexcept = default;

std::string Gadget::id() const { return impl_ ? impl_->id : "(moved-from)"; }
