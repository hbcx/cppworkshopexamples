#include <iostream>
#include <queue>
#include <string>
#include <utility>

// A payload that reports how it leaves the queue.
struct Job {
    std::string name;
    explicit Job(std::string n) : name(std::move(n)) {}
    Job(const Job& other) : name(other.name) {
        std::cout << "  copy of " << name << '\n';
    }
    Job(Job&& other) noexcept : name(std::move(other.name)) {
        std::cout << "  move of " << name << '\n';
    }
};

int main() {
    std::queue<Job> jobs;
    std::cout << "queueing:\n";
    jobs.push(Job("render"));
    jobs.push(Job("encode"));

    // The interface the standard does NOT give you:
    //   Job j = jobs.pop();   // would remove the element, THEN copy it out --
    //                         // if that copy threw, the job would be gone for good
    // Instead: front() hands out a reference (cannot throw), pop() only destroys
    // (cannot throw). Your copy happens in between, where a failure is harmless.

    std::cout << "copying out of front():\n";
    Job copied = jobs.front();          // queue still holds the original
    jobs.pop();                         // now, and only now, it is removed

    // For an expensive payload, move out instead -- the element is about to be
    // destroyed by pop() anyway. Order matters: move first, pop second.
    std::cout << "moving out of front():\n";
    Job moved = std::move(jobs.front());
    jobs.pop();                         // 'moved' owns the name; the husk is dropped

    std::cout << "got: " << copied.name << ", " << moved.name << '\n';
    std::cout << "remaining: " << jobs.size() << '\n';
    return 0;
}
