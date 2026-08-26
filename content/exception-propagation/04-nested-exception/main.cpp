// std::throw_with_nested attaches the exception currently being handled as the
// "cause" of a new one, so you can add CONTEXT at each layer without losing the
// original error. std::rethrow_if_nested walks back down the chain to recover it.
#include <iostream>
#include <exception>
#include <stdexcept>
#include <string>

// Low level: the real failure.
void read_setting() {
    throw std::runtime_error("disk read error");
}

// Mid level: catch and wrap with context, keeping the cause nested inside.
void load_config() {
    try {
        read_setting();
    }
    catch (...) {
        std::throw_with_nested(std::runtime_error("failed to load config"));
    }
}

// Print an exception and everything nested inside it, indented by depth.
void print_chain(const std::exception& e, int depth = 0) {
    std::cout << std::string(depth * 2, ' ') << "- " << e.what() << '\n';
    try {
        std::rethrow_if_nested(e);         // rethrow the cause, if there is one
    }
    catch (const std::exception& cause) {
        print_chain(cause, depth + 1);     // recurse into the cause
    }
}

int main() {
    try {
        load_config();
    }
    catch (const std::exception& e) {
        std::cout << "error chain:\n";
        print_chain(e);
    }
    return 0;
}
