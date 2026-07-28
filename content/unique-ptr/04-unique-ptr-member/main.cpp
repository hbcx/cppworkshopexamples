#include <iostream>
#include <memory>
#include <utility>

// A low-level resource with a noisy lifetime, so we can watch when it opens and
// closes.
class Socket {
public:
    explicit Socket(int fd) : fd_(fd) { std::cout << "socket " << fd_ << " open\n"; }
    ~Socket() { std::cout << "socket " << fd_ << " close\n"; }
    int fd() const { return fd_; }
private:
    int fd_;
};

// Rule of zero: the only resource is held in a unique_ptr member, so the
// compiler-generated destructor and move operations are already correct, and
// the class is move-only for free (copy is implicitly deleted). We write NONE
// of the five special members.
class Connection {
public:
    explicit Connection(int fd) : sock_(new Socket(fd)) {}
    int fd() const { return sock_->fd(); }
private:
    std::unique_ptr<Socket> sock_;
};

int main() {
    Connection c(7);
    std::cout << "c uses fd " << c.fd() << '\n';

    Connection moved = std::move(c);     // move ctor: the socket transfers, not reopened
    std::cout << "moved uses fd " << moved.fd() << '\n';

    // Connection copy = moved;          // would NOT compile: copy is deleted.
    std::cout << "end of main\n";
    return 0;
}   // moved is destroyed here -> the socket is closed exactly once
