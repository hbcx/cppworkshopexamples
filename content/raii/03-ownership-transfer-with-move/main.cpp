// A resource must have exactly ONE owner, or two destructors will release it
// twice. So a resource-owning RAII type is move-only: copying is disabled, and
// moving TRANSFERS the resource, leaving the source empty so its destructor
// does nothing -- take the source's handle, then null the source out.

#include <iostream>
#include <utility>   // std::move

int live = 0;

class Socket {
public:
    explicit Socket(int fd) : fd_(fd) {
        ++live;
        std::cout << "open  fd=" << fd_ << " (live=" << live << ")\n";
    }

    // Move constructor: steal the other's fd, null the other so it won't close.
    Socket(Socket&& other) noexcept : fd_(other.fd_) {
        other.fd_ = -1;
        std::cout << "move-construct fd=" << fd_ << "\n";
    }
    // Move assignment: release what we currently hold, then steal the other's.
    Socket& operator=(Socket&& other) noexcept {
        if (this != &other) {
            close();
            fd_ = other.fd_;
            other.fd_ = -1;
            std::cout << "move-assign  fd=" << fd_ << "\n";
        }
        return *this;
    }
    // No copying: a socket has a single owner.
    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    ~Socket() { close(); }

private:
    void close() {
        if (fd_ != -1) {            // a moved-from socket holds -1 and closes nothing
            --live;
            std::cout << "close fd=" << fd_ << " (live=" << live << ")\n";
            fd_ = -1;
        }
    }
    int fd_;
};

int main() {
    Socket a(42);                 // a owns fd 42
    Socket b = std::move(a);      // move-construct: 42 moves to b, a is now empty
    std::cout << "-- a is moved-from, b owns 42 --\n";

    Socket c(7);                  // c owns fd 7
    c = std::move(b);             // move-assign: c releases 7, then takes 42; b empty
    std::cout << "-- c owns 42 now, 7 was closed --\n";
}   // c closes 42; b and a own nothing, so they close nothing
