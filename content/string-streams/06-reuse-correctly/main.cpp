#include <iostream>
#include <sstream>
#include <string>

int main() {
    // To reuse a stringstream for a new string, do BOTH: str() to replace the
    // contents, and clear() to reset the state bits (a previous read to the end
    // left eofbit/failbit set, which would block the next operation).
    std::istringstream in;

    std::string data[] = { "1 2 3", "40 50", "600" };
    long total = 0;
    for (const std::string& s : data) {
        in.clear();      // reset eof/fail from the previous pass
        in.str(s);       // load the new content
        int x = 0;
        while (in >> x)
            total += x;
    }
    std::cout << "total: " << total << '\n';   // 1+2+3+40+50+600 = 696
    return 0;
}
