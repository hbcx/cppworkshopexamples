#include <iostream>
#include <sstream>

int main() {
    // There are four predefined streams:
    //   std::cout  -- standard output (stdout), buffered: normal program output
    //   std::cerr  -- standard error (stderr), UNbuffered: urgent error messages
    //   std::clog  -- standard error (stderr), buffered: diagnostic logging
    //   std::cin   -- standard input (stdin)
    // cout and cerr/clog go to DIFFERENT destinations, so a shell can separate
    // them: `prog > data.txt` keeps data in the file and errors on the screen.
    std::cout << "this is data on stdout\n";

    // cerr and clog go to stderr. To SHOW their text here (the harness records
    // stdout), we temporarily redirect cerr into a string and print that.
    std::ostringstream captured;
    std::streambuf* oldCerr = std::cerr.rdbuf(captured.rdbuf());
    std::cerr << "this is an error on stderr\n";
    std::cerr.rdbuf(oldCerr);   // restore

    std::cout << "captured from stderr: " << captured.str();
    return 0;
}
