#include <iostream>
#include <memory>

// make_unique (C++14) constructs the object and wraps it in a unique_ptr in one
// call. Its arguments are forwarded to the constructor.
class Session {
public:
    Session(int id, const char* user) : id_(id), user_(user) {
        std::cout << "Session " << id_ << " for " << user_ << " opened\n";
    }
    ~Session() {
        std::cout << "Session " << id_ << " closed\n";
    }
private:
    int id_;
    const char* user_;
};

int main() {
    // Preferred: no raw new, "Session" named once, exception-safe.
    auto s1 = std::make_unique<Session>(1, "alice");

    // The verbose equivalent: the type appears twice and a raw new briefly
    // exists between the allocation and the unique_ptr taking ownership.
    std::unique_ptr<Session> s2(new Session(2, "bob"));

    std::cout << "both sessions are live\n";
    // Both are released automatically, in reverse order of construction.
    return 0;
}
