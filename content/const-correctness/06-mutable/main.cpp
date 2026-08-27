#include <iostream>
#include <string>
#include <utility>

// A const member function cannot change the object -- except members marked
// mutable. mutable is for state that is not part of the object's observable
// value: a cache, a computed-once result, a lock. The object is still logically
// const (its value does not change), even though a hidden field does.

class Report {
public:
    explicit Report(std::string text) : text_(std::move(text)) {}

    // Logically const: the report's text never changes. The line count is
    // computed once and then served from a mutable cache.
    std::size_t lineCount() const {
        if (!counted_) {
            lines_ = 1;
            for (char c : text_) if (c == '\n') ++lines_;
            counted_ = true;
            std::cout << "  (computed line count)\n";
        } else {
            std::cout << "  (served from cache)\n";
        }
        return lines_;
    }

private:
    std::string         text_;
    mutable std::size_t lines_ = 0;      // cache -- may change in a const method
    mutable bool        counted_ = false;
};

int main() {
    const Report r("a\nb\nc");           // a CONST object
    std::cout << "lines = " << r.lineCount() << "\n";   // computes
    std::cout << "lines = " << r.lineCount() << "\n";   // uses the cache
    return 0;
}
