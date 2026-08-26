// The most useful pattern: capture the stack at the THROW site and carry it in
// the exception, so the handler -- far from where the error happened -- can log
// exactly how the program got there. A normal exception tells you WHAT; attaching
// a stacktrace tells you WHERE from.
#include <stacktrace>
#include <iostream>
#include <stdexcept>
#include <string>

class TracedError : public std::runtime_error {
public:
    explicit TracedError(const std::string& msg)
        : std::runtime_error(msg),
          trace_(std::stacktrace::current()) {}   // snapshot at the throw site
    const std::stacktrace& where() const noexcept { return trace_; }
private:
    std::stacktrace trace_;
};

void deep()   { throw TracedError("widget failed to load"); }
void middle() { deep(); }

int main() {
    std::cout << std::boolalpha;
    try {
        middle();
    }
    catch (const TracedError& e) {
        std::cout << "message: " << e.what() << '\n';
        // The trace was captured three calls deep, at the throw.
        std::cout << "trace captured: " << !e.where().empty() << '\n';   // true
        // In a real program you would write e.where() to the crash log here.
    }
    return 0;
}
