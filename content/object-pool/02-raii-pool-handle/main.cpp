#include <iostream>
#include <memory>
#include <string>
#include <vector>

struct Connection {
    int id;
    explicit Connection(int id_) : id(id_) {
        std::cout << "  constructed Connection #" << id << '\n';
    }
    void send(const std::string& msg) const {
        std::cout << "  Connection #" << id << " sends: " << msg << '\n';
    }
};

class ConnectionPool {
    // The custom deleter: instead of freeing the object it returns it to the
    // pool. As a nested class it may call the private release() directly.
    struct Return {
        ConnectionPool* pool;
        void operator()(Connection* c) const { pool->release(c); }
    };

public:
    // A handle is a unique_ptr whose "deletion" is a return-to-pool: when it
    // goes out of scope the object is recycled, not destroyed -- on every exit
    // path, exceptions included.
    using Handle = std::unique_ptr<Connection, Return>;

    Handle acquire() {
        Connection* c;
        if (!free_.empty()) {
            c = free_.back();
            free_.pop_back();
        } else {
            owned_.push_back(std::make_unique<Connection>(nextId_++));
            c = owned_.back().get();
        }
        return Handle(c, Return{this});
    }
    std::size_t created() const { return owned_.size(); }

private:
    void release(Connection* c) {
        std::cout << "  returned Connection #" << c->id << " to pool\n";
        free_.push_back(c);
    }
    std::vector<std::unique_ptr<Connection>> owned_;
    std::vector<Connection*> free_;
    int nextId_ = 1;
};

int main() {
    ConnectionPool pool;
    {
        ConnectionPool::Handle a = pool.acquire();
        a->send("hello");
        {
            ConnectionPool::Handle b = pool.acquire();
            b->send("world");
        }   // b's scope ends: Connection #2 returns to the pool automatically

        // This acquire reuses #2 -- no new construction.
        ConnectionPool::Handle c = pool.acquire();
        c->send("reused #2");
    }   // c then a return here, in reverse order of declaration

    std::cout << "objects ever constructed: " << pool.created() << '\n';
    return 0;
}
