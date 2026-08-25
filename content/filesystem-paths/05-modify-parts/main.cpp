#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int main() {
    // These members return a changed path without touching the disk -- handy
    // for turning one filename into a related one.
    fs::path src = "src/engine/render.cpp";

    fs::path obj = src;
    obj.replace_extension(".o");                 // src/engine/render.o
    std::cout << "replace_extension: " << obj.generic_string() << '\n';

    fs::path sibling = src;
    sibling.replace_filename("render.hpp");      // src/engine/render.hpp
    std::cout << "replace_filename:  " << sibling.generic_string() << '\n';

    fs::path dir = src;
    dir.remove_filename();                       // src/engine/
    std::cout << "remove_filename:   " << dir.generic_string() << '\n';

    // A common real job: map a source file to its build output in another tree,
    // keeping the stem and swapping the directory and the extension.
    fs::path out = fs::path("build/obj") / src.filename();
    out.replace_extension(".o");                 // build/obj/render.o
    std::cout << "build output:      " << out.generic_string() << '\n';
    return 0;
}
