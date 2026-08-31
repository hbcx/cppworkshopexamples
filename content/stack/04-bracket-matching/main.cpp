// Matching nested brackets is the textbook stack algorithm: push each opener,
// and on each closer pop the top and check it is the matching kind. A plain
// counter cannot do this -- it does not know WHICH bracket is open.

#include <iostream>
#include <stack>
#include <string>

static bool matches(char open, char close) {
    return (open == '(' && close == ')')
        || (open == '[' && close == ']')
        || (open == '{' && close == '}');
}

static bool balanced(const std::string& text) {
    std::stack<char> open;
    for (char c : text) {
        if (c == '(' || c == '[' || c == '{') {
            open.push(c);
        } else if (c == ')' || c == ']' || c == '}') {
            if (open.empty() || !matches(open.top(), c)) return false;
            open.pop();
        }
    }
    return open.empty();   // nothing left unclosed
}

int main() {
    const std::string samples[] = {"()[]{}", "([{}])", "(]", "([)]", "((("};
    for (const std::string& s : samples) {
        std::cout << s << "  ->  "
                  << (balanced(s) ? "balanced" : "not balanced") << '\n';
    }
}
