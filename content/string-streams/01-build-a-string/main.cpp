#include <iostream>
#include <sstream>
#include <string>

int main() {
    // std::ostringstream builds a string in memory the same way you write to
    // std::cout: stream values in with <<, then take the result with .str().
    std::ostringstream out;
    int width = 1920, height = 1080;
    out << "resolution " << width << "x" << height
        << " (" << (width * height) << " px)";

    std::string text = out.str();
    std::cout << text << '\n';
    std::cout << "length: " << text.size() << '\n';
    return 0;
}
