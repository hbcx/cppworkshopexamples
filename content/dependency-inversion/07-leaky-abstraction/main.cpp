// Anti-pattern: a leaky abstraction (the abstraction depends on a detail).
//
// UserRepository is an interface, but findUser returns a SqlRow -- a
// database-shaped type. Callers depend through the interface on a SQL type: they
// must parse it, they break if the format changes, and swapping the backend
// changes the return type and breaks everyone.
//
// The fix: the interface returns a domain User; each adapter translates its own
// storage format behind the port.

#include <iostream>
#include <string>

// ---------- the interface leaks a SQL-shaped type ----------
namespace bad {

struct SqlRow { std::string columns; }; // "id|name", the DB's representation

class UserRepository {
public:
    virtual ~UserRepository() {}
    virtual SqlRow findUser(int id) = 0; // leaks SqlRow across the boundary
};

class MySqlUsers : public UserRepository {
public:
    SqlRow findUser(int id) override {
        return SqlRow{std::to_string(id) + "|Ada"};
    }
};

// The caller is coupled to the SQL representation and must parse it.
std::string greet(UserRepository& repo, int id) {
    SqlRow row = repo.findUser(id);
    std::string name = row.columns.substr(row.columns.find('|') + 1);
    return "Hello " + name;
}

} // namespace bad

// ---------- the interface speaks in a domain type ----------
namespace good {

struct User { int id; std::string name; }; // a domain type

class UserRepository {
public:
    virtual ~UserRepository() {}
    virtual User findUser(int id) = 0;
};

class MySqlUsers : public UserRepository {
public:
    User findUser(int id) override {
        // Parsing the SQL row stays INSIDE the adapter.
        return User{id, "Ada"};
    }
};

// The caller depends only on User -- no storage format in sight.
std::string greet(UserRepository& repo, int id) {
    return "Hello " + repo.findUser(id).name;
}

} // namespace good

int main() {
    bad::MySqlUsers badRepo;
    std::cout << "[bad]  " << bad::greet(badRepo, 1)
              << " (caller had to parse a SqlRow)\n";

    good::MySqlUsers repo;
    std::cout << "[good] " << good::greet(repo, 1)
              << " (caller sees only a User)\n";
}
