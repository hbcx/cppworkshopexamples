#include <iostream>
#include <memory>
#include <vector>

// C++17 added weak_from_this() to enable_shared_from_this. A live object can
// register a NON-OWNING reference to itself with some manager -- the manager
// observes it and never keeps it alive.
class Job : public std::enable_shared_from_this<Job> {
public:
    explicit Job(int id) : id_(id) {}
    ~Job() { std::cout << "Job " << id_ << " destroyed\n"; }
    void watch(std::vector<std::weak_ptr<Job>>& board) {
        board.push_back(weak_from_this());       // weak self-reference (C++17)
    }
    int id() const { return id_; }
private:
    int id_;
};

int main() {
    std::vector<std::weak_ptr<Job>> board;       // a watch list that owns nothing
    {
        auto job = std::make_shared<Job>(1);
        job->watch(board);
        std::cout << "watched; use_count = " << job.use_count() << '\n';   // 1: weak did not raise it
        if (std::shared_ptr<Job> live = board[0].lock()) {
            std::cout << "board sees live Job " << live->id() << '\n';
        }
    }   // the only owner goes -> Job destroyed, though the board still lists it

    std::cout << "board entry expired? " << (board[0].expired() ? "yes" : "no") << '\n';  // yes
    return 0;
}
