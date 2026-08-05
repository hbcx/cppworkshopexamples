// Anti-pattern: high-level policy tied to a concrete low-level detail.
//
// ReportService is policy, but it owns and constructs a concrete MySqlDatabase,
// so the dependency arrow points down at the detail: the policy needs MySQL to
// compile or test, a swap means editing the policy, and a change to the DB
// ripples up into the report logic.
//
// The fix: depend on a Database abstraction and receive the concrete one, so the
// arrow points at the abstraction from both sides.

#include <iostream>
#include <string>
#include <vector>

// ---------- policy welded to a concrete database ----------
namespace bad {

class MySqlDatabase {
public:
    std::vector<std::string> query() { return std::vector<std::string>(3, "row"); }
};

class ReportService {
public:
    std::string run() {
        std::vector<std::string> rows = db_.query();
        return "report of " + std::to_string(rows.size()) + " rows";
    }
private:
    MySqlDatabase db_; // concrete detail hard-wired into the policy
};

} // namespace bad

// ---------- policy depends on an abstraction ----------
namespace good {

class Database {
public:
    virtual ~Database() {}
    virtual std::vector<std::string> query() = 0;
};

class MySqlDatabase : public Database {
public:
    std::vector<std::string> query() override { return std::vector<std::string>(3, "row"); }
};

// A fake used by a test -- possible only because policy depends on Database.
class FakeDatabase : public Database {
public:
    std::vector<std::string> query() override { return std::vector<std::string>(1, "row"); }
};

class ReportService {
public:
    explicit ReportService(Database& db) : db_(db) {}
    std::string run() {
        std::vector<std::string> rows = db_.query();
        return "report of " + std::to_string(rows.size()) + " rows";
    }
private:
    Database& db_;
};

} // namespace good

int main() {
    bad::ReportService legacy;
    std::cout << "[bad]  " << legacy.run() << "\n";

    good::MySqlDatabase mysql;
    good::ReportService svc(mysql);
    std::cout << "[good] " << svc.run() << "\n";

    good::FakeDatabase fake;
    good::ReportService tested(fake);
    std::cout << "[good] with fake: " << tested.run() << "\n";
}
