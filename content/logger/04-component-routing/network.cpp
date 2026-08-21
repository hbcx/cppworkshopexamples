#include "network.hpp"

#include "logger.hpp"

namespace network {

void connect(const char* host, int port) {
    log_network<Level::Info, Severity::Minor>("connecting to {}:{}", host, port);
    log_network<Level::Error, Severity::Critical>("handshake with {} failed", host);
}

} // namespace network
