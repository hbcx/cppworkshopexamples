#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

int main() {
    // Anti-pattern: treating last_write_time's result as a system_clock time and
    // feeding it to system_clock::to_time_t or subtracting a system_clock value
    // from it. last_write_time returns a file_time_type, which is on a DIFFERENT
    // clock (std::chrono::file_clock) with its own epoch. Mixing clocks does not
    // even compile when you are lucky, and gives nonsense when a cast lets it
    // through.
    //
    //   auto ft = fs::last_write_time(p);
    //   std::time_t t = std::chrono::system_clock::to_time_t(ft);  // WRONG: not system_clock
    //
    // (In C++20 you convert with std::chrono::clock_cast<system_clock>(ft); in
    // C++17 there is no portable conversion -- the file_clock epoch is
    // implementation-defined.)
    fs::path root = fs::temp_directory_path() / "cppws_fsmt";
    std::error_code ec;
    fs::remove_all(root, ec);
    fs::create_directories(root);
    fs::path older = root / "old.txt";
    fs::path newer = root / "new.txt";
    std::ofstream(older) << "a";
    std::ofstream(newer) << "b";

    // Fix: stay on ONE clock. Compare file times against each other -- both are
    // file_time_type -- to answer "which is newer", the usual real question.
    fs::last_write_time(newer, fs::last_write_time(older) + std::chrono::hours(2));
    bool newerIsNewer = fs::last_write_time(newer) > fs::last_write_time(older);
    std::cout << "newer is newer: " << newerIsNewer << '\n';   // 1

    // And an interval BETWEEN two file times is a normal chrono duration.
    auto gap = fs::last_write_time(newer) - fs::last_write_time(older);
    std::cout << "gap in hours: "
              << std::chrono::duration_cast<std::chrono::hours>(gap).count() << '\n'; // 2

    fs::remove_all(root, ec);
    return 0;
}
