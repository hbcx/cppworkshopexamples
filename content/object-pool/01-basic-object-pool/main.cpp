#include <iostream>
#include <memory>
#include <string>
#include <vector>

// A resource that is expensive to create. Here we just tag each one with a
// stable id and print on construction, so the output proves how many were ever
// built -- the number the pool exists to keep small.
struct Connection {
    int id;
    explicit Connection(int id_) : id(id_) {
        std::cout << "  constructed Connection #" << id << '\n';
    }
    void send(const std::string& msg) const {
        std::cout << "  Connection #" << id << " sends: " << msg << '\n';
    }
};

// The pool owns every object and hands out raw pointers to the free ones.
// acquire() reuses a free object, constructing a new one only when none are
// free; release() puts an object back on the free list.
class ConnectionPool {
public:
    Connection* acquire() {
        if (!free_.empty()) {
            Connection* c = free_.back();   // LIFO: last released, first reused
            free_.pop_back();
            return c;
        }
        owned_.push_back(std::make_unique<Connection>(nextId_++));
        return owned_.back().get();
    }
    void release(Connection* c) { free_.push_back(c); }
    std::size_t created() const { return owned_.size(); }

private:
    std::vector<std::unique_ptr<Connection>> owned_;  // owns every object
    std::vector<Connection*> free_;                   // non-owning: ready to reuse
    int nextId_ = 1;
};

int main() {
    ConnectionPool pool;

    // Acquire two, use them, release them back to the pool.
    Connection* a = pool.acquire();
    Connection* b = pool.acquire();
    a->send("hello");
    b->send("world");
    pool.release(a);
    pool.release(b);

    // Acquire two more: the pool reuses the objects it already made -- no new
    // Connection is constructed, and the ids come back (b was released last, so
    // it is handed out first).
    Connection* c = pool.acquire();
    Connection* d = pool.acquire();
    c->send("again");
    d->send("reused");
    pool.release(c);
    pool.release(d);

    std::cout << "objects ever constructed: " << pool.created() << '\n';
    return 0;
}
