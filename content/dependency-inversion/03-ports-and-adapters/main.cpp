// Ports and adapters (hexagonal architecture): Dependency Inversion for a whole
// application.
//
// The core Registration owns a UserStore PORT. Two ADAPTERS implement it: an
// in-memory store and one that logs like a database. The core runs identically
// against either, and a test can drive it with the in-memory adapter. All the
// concrete technology sits in adapters around a core that depends on no I/O.

#include <iostream>
#include <string>
#include <vector>

// --- the core owns this port, in its own terms ---
class UserStore {
public:
    virtual ~UserStore() {}
    virtual void save(const std::string& name) = 0;
    virtual int count() const = 0;
};

// --- core policy: depends only on the port ---
class Registration {
public:
    explicit Registration(UserStore& store) : store_(store) {}
    std::string registerUser(const std::string& name) {
        store_.save(name);
        return name + " registered (total " + std::to_string(store_.count()) + ")";
    }
private:
    UserStore& store_;
};

// --- adapter 1: in-memory (great for tests) ---
class MemoryStore : public UserStore {
public:
    void save(const std::string& name) override { users_.push_back(name); }
    int count() const override { return static_cast<int>(users_.size()); }
private:
    std::vector<std::string> users_;
};

// --- adapter 2: stands in for a database, logging each insert ---
class DbStore : public UserStore {
public:
    void save(const std::string& name) override {
        std::cout << "[db] INSERT user " << name << "\n";
        ++rows_;
    }
    int count() const override { return rows_; }
private:
    int rows_ = 0;
};

int main() {
    MemoryStore memory;
    Registration core1(memory);
    std::cout << core1.registerUser("ada") << "\n";
    std::cout << core1.registerUser("grace") << "\n";

    // Same core, different adapter -- no change to Registration.
    DbStore db;
    Registration core2(db);
    std::cout << core2.registerUser("linus") << "\n";
}
