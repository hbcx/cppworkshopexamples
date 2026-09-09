#include <chrono>
#include <iostream>

namespace very { namespace deeply { namespace nested {
    int answer() { return 42; }
}}}

int main() {
    namespace vdn = very::deeply::nested;   // a short local name for a long one
    std::cout << "answer = " << vdn::answer() << "\n";

    namespace ch = std::chrono;             // also handy for standard nested namespaces
    ch::milliseconds ms{250};
    std::cout << "ms = " << ms.count() << "\n";
    return 0;
}
