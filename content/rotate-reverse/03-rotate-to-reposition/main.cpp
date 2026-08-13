#include <algorithm>
#include <string>
#include <vector>
#include <iostream>

int main() {
    std::vector<std::string> playlist{"a", "b", "c", "d", "E", "f"};

    // Move the item at index 4 ("E") to index 1 -- a drag-and-drop reorder. rotate
    // on the sub-range [to, from+1) brings the element at `from` to `to` and shifts
    // the elements in between one step to the right, all in one in-place pass.
    std::size_t from = 4, to = 1;
    std::rotate(playlist.begin() + to,
                playlist.begin() + from,
                playlist.begin() + from + 1);
    std::cout << "move index 4 to 1: ";
    for (const auto& s : playlist) std::cout << s << ' ';
    std::cout << '\n';

    // Moving an element the OTHER way (earlier -> later) uses middle = from+1: the
    // one element wraps to the end of the sub-range. Move index 1 to index 3.
    std::vector<std::string> q{"a", "B", "c", "d", "e"};
    std::rotate(q.begin() + 1, q.begin() + 2, q.begin() + 4);
    std::cout << "move index 1 to 3: ";
    for (const auto& s : q) std::cout << s << ' ';
    std::cout << '\n';
    return 0;
}
