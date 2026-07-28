#include <iostream>
#include <memory>

class Session {
public:
    Session(int id, const char* user) : id_(id), user_(user) {
        std::cout << "Session " << id_ << " for " << user_ << " opened\n";
    }
    ~Session() { std::cout << "Session " << id_ << " closed\n"; }
private:
    int id_;
    const char* user_;
};

int main() {
    // Preferred: ONE allocation for the object and its control block together,
    // exception-safe, and the type is named once.
    auto s1 = std::make_shared<Session>(1, "alice");

    // The verbose form: two allocations (the object, then a separate control
    // block) and a raw new that briefly owns nothing.
    std::shared_ptr<Session> s2(new Session(2, "bob"));

    std::cout << "s1 use_count = " << s1.use_count() << '\n';   // 1
    std::cout << "both sessions are live\n";
    return 0;   // closed in reverse order of construction
}
