#include <iostream>
#include <memory>

// Pretend this is a C library: it hands out an opaque handle that must be
// released with a specific free function, never with delete.
struct DbConn {
    int id;
};

static DbConn* db_open(int id) {
    std::cout << "db_open " << id << '\n';
    return new DbConn{id};
}

static void db_close(DbConn* c) {
    std::cout << "db_close " << c->id << '\n';
    delete c;
}

// A stateless function-object deleter: it holds no data, so the empty base
// optimization keeps the unique_ptr the size of a single pointer.
struct DbCloser {
    void operator()(DbConn* c) const { db_close(c); }
};
using DbPtr = std::unique_ptr<DbConn, DbCloser>;

// A function-pointer deleter must be stored inside the unique_ptr, which makes
// it two pointers wide.
using DbPtrFn = std::unique_ptr<DbConn, void (*)(DbConn*)>;

int main() {
    {
        DbPtr c(db_open(1));                 // released via DbCloser at scope end
        std::cout << "using connection " << c->id << '\n';
    }
    {
        DbPtrFn c(db_open(2), db_close);     // deleter passed as a value
        std::cout << "using connection " << c->id << '\n';
    }

    std::cout << "sizeof(void*)   = " << sizeof(void*) << '\n';
    std::cout << "sizeof(DbPtr)   = " << sizeof(DbPtr) << '\n';    // == one pointer
    std::cout << "sizeof(DbPtrFn) = " << sizeof(DbPtrFn) << '\n';  // == two pointers
    return 0;
}
