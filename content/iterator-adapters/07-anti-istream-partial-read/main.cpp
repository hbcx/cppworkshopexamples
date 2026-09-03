#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

int main() {
    // ANTI-PATTERN: assuming istream_iterator reads the WHOLE input. It stops at
    // the first item that fails to parse as the requested type, silently -- the
    // rest of the stream is left unread and no exception is raised.
    std::istringstream in("10 20 oops 40 50");
    std::istream_iterator<int> begin(in), end;
    std::vector<int> nums(begin, end);

    std::cout << "read " << nums.size() << " ints:";   // 2, not 5 -- stops at "oops"
    for (int x : nums) std::cout << " " << x;
    std::cout << "\n";

    // The stream is now in a failed state; you must CHECK it, not trust the count.
    std::cout << std::boolalpha << "stream failed? " << in.fail() << "\n";   // true

    // FIX: when the input may be malformed, read tokens as strings and convert
    // each one yourself, so a bad token is detected instead of ending the range.
    std::istringstream in2("10 20 oops 40 50");
    std::istream_iterator<std::string> b2(in2), e2;
    int good = 0, bad = 0;
    for (auto it = b2; it != e2; ++it) {
        try { (void)std::stoi(*it); ++good; }
        catch (const std::exception&) { ++bad; }
    }
    std::cout << "as strings -> good=" << good << " bad=" << bad << "\n";   // good=4 bad=1
    return 0;
}
