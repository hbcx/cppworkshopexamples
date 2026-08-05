// Anti-pattern: a fat interface forces fat test doubles.
//
// greet() uses one method of IDatabase (get), but a fake for it must implement
// all five -- four are useless stubs that exist only to compile, and the fake
// breaks whenever a method is added to IDatabase.
//
// The fix: greet() depends on a narrow UserLookup with just get(), so the fake
// is a single method and unrelated storage changes cannot break the test.

#include <iostream>
#include <string>

// ---------- a wide interface forces a wide fake ----------
namespace bad {

class IDatabase {
public:
    virtual ~IDatabase() {}
    virtual std::string get(int id) const = 0;
    virtual void put(int id, const std::string& v) = 0;
    virtual void remove(int id) = 0;
    virtual void flush() = 0;
    virtual int count() const = 0;
};

// Code under test needs only get().
std::string greet(const IDatabase& db, int id) { return "Hello " + db.get(id); }

// ...yet the fake must implement everything, four stubs for nothing.
class FakeDatabase : public IDatabase {
public:
    std::string get(int) const override { return "Ada"; }
    void put(int, const std::string&) override {}   // unused stub
    void remove(int) override {}                     // unused stub
    void flush() override {}                          // unused stub
    int count() const override { return 0; }          // unused stub
};

} // namespace bad

// ---------- a narrow interface makes the fake trivial ----------
namespace good {

class UserLookup {
public:
    virtual ~UserLookup() {}
    virtual std::string get(int id) const = 0;
};

std::string greet(const UserLookup& users, int id) { return "Hello " + users.get(id); }

// The whole fake -- one method, nothing to add when storage grows elsewhere.
class FakeLookup : public UserLookup {
public:
    std::string get(int) const override { return "Ada"; }
};

} // namespace good

int main() {
    bad::FakeDatabase fakeDb;
    std::cout << "[bad]  " << bad::greet(fakeDb, 1) << " (fake needed 5 methods)\n";

    good::FakeLookup fakeLookup;
    std::cout << "[good] " << good::greet(fakeLookup, 1) << " (fake needed 1 method)\n";
}
