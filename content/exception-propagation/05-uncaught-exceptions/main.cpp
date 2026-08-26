// std::uncaught_exceptions() (C++17) returns how many exceptions are currently
// "in flight" (being propagated). A scope guard captures the count at construction
// and compares at destruction: if it grew, the scope is being left because of an
// exception, so the guard rolls back instead of committing. This is how a
// transaction guard tells success from failure without being told.
#include <iostream>
#include <exception>
#include <stdexcept>

class TransactionGuard {
public:
    TransactionGuard() : entry_count_(std::uncaught_exceptions()) {}
    ~TransactionGuard() {
        if (std::uncaught_exceptions() > entry_count_) {
            std::cout << "  rollback (leaving via an exception)\n";
        } else {
            std::cout << "  commit (normal exit)\n";
        }
    }
private:
    int entry_count_;
};

void run(bool fail) {
    TransactionGuard guard;
    std::cout << (fail ? "work then throw:\n" : "work then return:\n");
    if (fail) throw std::runtime_error("work failed");
}

int main() {
    // Normal exit -> the guard commits.
    run(false);

    // Exceptional exit -> the same guard rolls back.
    try {
        run(true);
    }
    catch (...) {
        std::cout << "caught\n";
    }
    return 0;
}
