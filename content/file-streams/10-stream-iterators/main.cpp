#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

namespace fs = std::filesystem;

int main() {
    fs::path dir = fs::temp_directory_path() / "cppws_fs_iter";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);
    fs::path file = dir / "nums.txt";
    { std::ofstream(file) << "5 3 9 1 7\n"; }

    // Read every int straight from the file into a vector with istream_iterator.
    // The default-constructed iterator is the end-of-stream marker.
    std::ifstream in(file);
    std::vector<int> nums{ std::istream_iterator<int>(in),
                           std::istream_iterator<int>() };

    std::sort(nums.begin(), nums.end());

    // Write the sorted numbers to cout via ostream_iterator, space-separated,
    // with no explicit loop.
    std::copy(nums.begin(), nums.end(),
              std::ostream_iterator<int>(std::cout, " "));
    std::cout << '\n';
    std::cout << "count: " << nums.size() << '\n';   // 5

    fs::remove_all(dir, ec);
    return 0;
}
