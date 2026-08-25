#include <iostream>
#include <sstream>
#include <string>

int main() {
    // std::stringstream can be both written to and read from -- a handy
    // in-memory buffer. Write mixed values in, then read them back out in order.
    std::stringstream buf;
    buf << 10 << ' ' << 20 << ' ' << 30;

    int a = 0, b = 0, c = 0;
    buf >> a >> b >> c;
    std::cout << "read back: " << a << " " << b << " " << c << '\n';   // 10 20 30
    std::cout << "sum: " << (a + b + c) << '\n';                       // 60

    // The buffer's whole content is still available as a string.
    std::cout << "contents: " << buf.str() << '\n';   // 10 20 30
    return 0;
}
