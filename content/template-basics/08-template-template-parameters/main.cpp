#include <iostream>
#include <list>
#include <vector>

// A template TEMPLATE parameter takes a template (not a type) as its argument, so
// a class can be parameterised on WHICH container to use. template <class...>
// class matches std::vector, std::list and friends (they have extra parameters).
template <template <class...> class Container, class T>
class Stack {
public:
    void push(T x) { data_.push_back(x); }
    T pop() { T x = data_.back(); data_.pop_back(); return x; }
    bool empty() const { return data_.empty(); }

private:
    Container<T> data_;   // Container is a template; Container<T> instantiates it
};

int main() {
    Stack<std::vector, int> vs;   // backed by a std::vector
    vs.push(1); vs.push(2); vs.push(3);
    std::cout << "vector-backed pop: " << vs.pop() << " " << vs.pop() << "\n";   // 3 2

    Stack<std::list, int> ls;     // backed by a std::list, same Stack code
    ls.push(10); ls.push(20);
    std::cout << "list-backed pop:   " << ls.pop() << "\n";   // 20
    return 0;
}
