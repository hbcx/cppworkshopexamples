#include <iostream>
#include <string>

// A concrete database client with real behaviour.
class PostgresDb {
public:
    std::string query(const std::string& sql) { return "pg[" + sql + "]"; }
};

// Anti-pattern: the dependency IS injected -- but as the concrete PostgresDb.
// The seam is only skin deep: you can pass a different PostgresDb, but not a
// different KIND of database, and a test cannot substitute an in-memory fake
// because there is no interface to implement.
class ReportConcrete {
public:
    explicit ReportConcrete(PostgresDb& db) : db_(db) {}
    std::string run() { return db_.query("SELECT count(*)"); }
private:
    PostgresDb& db_;
};

// Fix: depend on an abstraction. Any implementation -- the real database or a
// fake -- satisfies the dependency, and the concrete type is the caller's
// choice, not the consumer's.
struct Database {
    virtual std::string query(const std::string& sql) = 0;
    virtual ~Database() = default;
};

struct PostgresAdapter : Database {
    std::string query(const std::string& sql) override { return "pg[" + sql + "]"; }
};

struct FakeDb : Database {
    std::string query(const std::string& sql) override { return "fake[" + sql + "]"; }
};

class ReportAbstract {
public:
    explicit ReportAbstract(Database& db) : db_(db) {}
    std::string run() { return db_.query("SELECT count(*)"); }
private:
    Database& db_;
};

int main() {
    // Concrete injection: tied to PostgresDb, cannot be given a fake.
    PostgresDb pg;
    ReportConcrete concrete(pg);
    std::cout << "concrete dep        -> " << concrete.run() << '\n';

    // Abstract injection: the same report runs against a fake in a test...
    FakeDb fake;
    ReportAbstract tested(fake);
    std::cout << "abstract dep (test) -> " << tested.run() << '\n';

    // ...and against the real adapter in production, with no change to the class.
    PostgresAdapter prod;
    ReportAbstract production(prod);
    std::cout << "abstract dep (prod) -> " << production.run() << '\n';
    return 0;
}
