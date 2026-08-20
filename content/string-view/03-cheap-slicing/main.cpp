#include <iostream>
#include <string>
#include <string_view>
#include <vector>

// Split on a single delimiter, returning VIEWS into the original buffer.
// No std::string is built for any field -- every token is a window into `text`.
static std::vector<std::string_view> split(std::string_view text, char delim) {
    std::vector<std::string_view> out;
    while (true) {
        std::size_t pos = text.find(delim);
        out.push_back(text.substr(0, pos));   // substr on a view: O(1), no copy
        if (pos == std::string_view::npos) break;
        text.remove_prefix(pos + 1);          // step the window past the delimiter
    }
    return out;
}

int main() {
    std::string csv = "id,name,role,city";

    for (std::string_view field : split(csv, ','))
        std::cout << '[' << field << "] ";
    std::cout << '\n';

    // The tokens point INTO csv -- prove it by showing their offsets in the buffer.
    std::vector<std::string_view> fields = split(csv, ',');
    std::cout << "fields: " << fields.size() << '\n';
    for (const std::string_view f : fields)
        std::cout << "  offset " << (f.data() - csv.data())
                  << ", len " << f.size() << " -> " << f << '\n';
    return 0;
}
