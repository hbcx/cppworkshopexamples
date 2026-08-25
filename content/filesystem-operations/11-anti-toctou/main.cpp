#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

int main() {
    // Anti-pattern: "check, then act" -- test exists() and then open/use the
    // path as if the answer still holds. Between the two calls another process
    // (or another thread) can delete or replace the file, so the check is a
    // promise the OS never made. This is a TOCTOU (time-of-check to time-of-use)
    // bug.
    fs::path root = fs::temp_directory_path() / "cppws_fstoctou";
    std::error_code ec;
    fs::remove_all(root, ec);
    fs::create_directories(root);
    fs::path f = root / "data.txt";
    std::ofstream(f) << "hi";

    // The check passes...
    bool checked = fs::exists(f);
    // ...but the world changes before we use it (here we delete it ourselves to
    // make the race deterministic; in real code someone else does).
    fs::remove(f);
    std::uintmax_t size = fs::file_size(f, ec);   // now fails
    (void) size;
    std::cout << "exists said: " << checked << '\n';                     // 1
    std::cout << "use failed anyway: " << static_cast<bool>(ec) << '\n'; // 1

    // Fix: do not pre-check. Attempt the operation and handle its error in one
    // step -- open the file and check the stream, or use the error_code form --
    // so there is no gap to lose the race in.
    std::ifstream in(f);
    std::cout << "open-and-check handled: " << !in.is_open() << '\n'; // 1 (cleanly failed)

    fs::remove_all(root, ec);
    return 0;
}
