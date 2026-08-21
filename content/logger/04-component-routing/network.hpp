#pragma once
// A network subsystem, in its own translation unit. It logs through log_network,
// so its records are tagged Component::Network and show network.cpp as their
// origin.
namespace network {
void connect(const char* host, int port);
}
