#include <iostream>
#include <memory>
#include <typeinfo>

// A C-style resource API: an opaque handle that must be released with a specific
// free function, never with delete.
struct DbConn { int id; };

static DbConn* db_open(int id) { std::cout << "db_open " << id << '\n'; return new DbConn{id}; }
static void db_close(DbConn* c) { std::cout << "db_close " << c->id << '\n'; delete c; }

int main() {
    // The deleter is stored in the control block (type-erased), NOT in the type,
    // so both of these are the SAME type, std::shared_ptr<DbConn>.
    std::shared_ptr<DbConn> a(db_open(1), db_close);          // function-pointer deleter
    std::shared_ptr<DbConn> b(db_open(2), [](DbConn* c) {      // lambda deleter
        db_close(c);
    });

    std::cout << "same type?  " << (typeid(a) == typeid(b) ? "yes" : "no") << '\n';
    std::cout << "same size?  " << (sizeof(a) == sizeof(b) ? "yes" : "no") << '\n';

    b = a;   // legal: same type. b drops conn 2 (its deleter runs), then shares conn 1
    std::cout << "conn 1 use_count = " << a.use_count() << '\n';   // 2
    return 0;   // conn 1 closed once, via its stored deleter
}
