#include "document.hpp"

struct Document::Impl {
    std::string title;
    std::string body;
};

Document::Document(std::string title) : impl_(std::make_unique<Impl>()) {
    impl_->title = std::move(title);
}

// All defined where Impl is complete, so destroying the old Impl is well-formed.
Document::~Document() = default;
Document::Document(Document&& other) noexcept = default;            // steals the pointer
Document& Document::operator=(Document&& other) noexcept = default;

void Document::append(const std::string& text) { impl_->body += text; }

std::string Document::title() const { return impl_ ? impl_->title : "(moved-from)"; }

bool Document::empty() const { return impl_ == nullptr; }
