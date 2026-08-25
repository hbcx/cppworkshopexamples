#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int main() {
    // temp_directory_path() is where to put scratch files -- it is always an
    // existing absolute directory. (Its actual location differs per machine, so
    // we check properties, not the exact text.)
    fs::path tmp = fs::temp_directory_path();
    std::cout << "temp is_absolute: " << tmp.is_absolute() << '\n';   // 1
    std::cout << "temp exists:      " << fs::exists(tmp) << '\n';     // 1

    // current_path() reads the working directory; the one-argument form CHANGES
    // it. Save it, move into a scratch dir, then restore it -- leaving the cwd
    // where you found it is good manners.
    fs::path saved = fs::current_path();
    fs::path work = tmp / "cppws_fscwd";
    std::error_code ec;
    fs::remove_all(work, ec);
    fs::create_directories(work);

    fs::current_path(work);
    std::cout << "moved into work:  "
              << (fs::current_path().filename() == "cppws_fscwd") << '\n'; // 1

    fs::current_path(saved);   // restore
    std::cout << "restored:         " << (fs::current_path() == saved) << '\n'; // 1

    fs::remove_all(work, ec);
    return 0;
}
