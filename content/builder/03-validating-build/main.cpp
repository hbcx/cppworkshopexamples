#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

// The product: reachable EITHER over TCP (host + port) OR a Unix socket, never
// both and never neither. Immutable, and only ever produced by a valid build().
class DatabaseConfig {
public:
    class Builder;
    void print() const {
        if (!socket_.empty())
            std::cout << "config: unix socket " << socket_ << '\n';
        else
            std::cout << "config: tcp " << host_ << ':' << port_ << '\n';
    }

private:
    DatabaseConfig(std::string host, int port, std::string socket)
        : host_(std::move(host)), port_(port), socket_(std::move(socket)) {}
    friend class Builder;

    std::string host_;
    int port_ = 0;
    std::string socket_;
};

class DatabaseConfig::Builder {
public:
    Builder& tcp(std::string host, int port) {
        host_ = std::move(host);
        port_ = port;
        return *this;
    }
    Builder& unixSocket(std::string path) {
        socket_ = std::move(path);
        return *this;
    }

    // The single checkpoint: cross-field invariants are enforced here, so an
    // inconsistent DatabaseConfig can never be constructed.
    DatabaseConfig build() const {
        const bool hasTcp = !host_.empty();
        const bool hasSocket = !socket_.empty();
        if (hasTcp == hasSocket)
            throw std::invalid_argument("set exactly one of tcp() or unixSocket()");
        if (hasTcp && (port_ < 1 || port_ > 65535))
            throw std::invalid_argument("tcp port out of range");
        return DatabaseConfig(host_, port_, socket_);
    }

private:
    std::string host_;
    int port_ = 0;
    std::string socket_;
};

int main() {
    // Valid: exactly one transport, port in range.
    DatabaseConfig::Builder().tcp("db.internal", 5432).build().print();

    // Invalid: neither transport set. build() refuses to make the object.
    try {
        DatabaseConfig::Builder().build();
    } catch (const std::invalid_argument& e) {
        std::cout << "rejected: " << e.what() << '\n';
    }

    // Invalid: a TCP port out of range -- the second cross-field guard fires.
    try {
        DatabaseConfig::Builder().tcp("db.internal", 70000).build();
    } catch (const std::invalid_argument& e) {
        std::cout << "rejected: " << e.what() << '\n';
    }
    return 0;
}
