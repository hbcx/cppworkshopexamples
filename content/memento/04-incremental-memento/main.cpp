// Incremental mementos: store the change, not the whole state.
//
// A full snapshot copies the entire state on every save, which is wasteful when
// the state is large and each change touches only a little of it. An incremental
// (delta) memento records just what changed -- here, the single cell's index and
// its old value -- so undo reverts that one cell. The saving is the difference
// between copying the whole grid per edit and copying one cell.

#include <iostream>
#include <vector>
#include <cstddef>

class Grid { // originator with large state
    std::vector<int> cells_;
public:
    explicit Grid(std::size_t n) : cells_(n, 0) {}
    int get(std::size_t i) const { return cells_[i]; }
    std::size_t size() const { return cells_.size(); }

    // A delta memento: only the one cell that changed, and its old value.
    class Delta {
        std::size_t index_;
        int oldValue_;
        Delta(std::size_t i, int old) : index_(i), oldValue_(old) {}
        friend class Grid;
    };

    Delta set(std::size_t i, int v) {
        Delta d(i, cells_[i]); // capture only the old value of this cell
        cells_[i] = v;
        return d;
    }
    void undo(const Delta& d) { cells_[d.index_] = d.oldValue_; }
};

int main() {
    Grid grid(1000);
    std::vector<Grid::Delta> history;

    history.push_back(grid.set(10, 5));
    history.push_back(grid.set(500, 9));
    history.push_back(grid.set(10, 7));
    std::cout << "cell10=" << grid.get(10) << " cell500=" << grid.get(500) << "\n"; // 7, 9

    // Undo in reverse: each delta reverts exactly one cell.
    for (std::vector<Grid::Delta>::reverse_iterator it = history.rbegin(); it != history.rend(); ++it)
        grid.undo(*it);
    std::cout << "after undo: cell10=" << grid.get(10) << " cell500=" << grid.get(500) << "\n"; // 0, 0

    std::cout << "per-change delta stores 1 cell; a full snapshot would store "
              << grid.size() << " cells\n";
    return 0;
}
