#ifndef PIMPL_MOVE_DOCUMENT_HPP
#define PIMPL_MOVE_DOCUMENT_HPP

#include <memory>
#include <string>

// A move-only pimpl: the move operations are declared here, defined in the .cpp.
class Document {
public:
    explicit Document(std::string title);
    ~Document();
    Document(Document&& other) noexcept;            // declared here...
    Document& operator=(Document&& other) noexcept; // ...defined in the .cpp
    // (no copy declared -> copy stays deleted by the unique_ptr member)

    void append(const std::string& text);
    std::string title() const;
    bool empty() const; // true once moved-from

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

#endif
