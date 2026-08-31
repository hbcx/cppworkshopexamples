// A reverse Polish notation (postfix) evaluator is one stack loop: push each
// number, and on each operator pop the top two operands, apply it, and push the
// result. No parentheses, no precedence -- the token order encodes the grouping.

#include <iostream>
#include <stack>
#include <string>
#include <vector>

static bool isOperator(const std::string& t) {
    return t == "+" || t == "-" || t == "*" || t == "/";
}

static int evalRPN(const std::vector<std::string>& tokens) {
    std::stack<int> operands;
    for (const std::string& t : tokens) {
        if (isOperator(t)) {
            int rhs = operands.top(); operands.pop();   // first popped = right side
            int lhs = operands.top(); operands.pop();
            int result = 0;
            if (t == "+") result = lhs + rhs;
            else if (t == "-") result = lhs - rhs;
            else if (t == "*") result = lhs * rhs;
            else result = lhs / rhs;
            operands.push(result);
        } else {
            operands.push(std::stoi(t));
        }
    }
    return operands.top();   // the single remaining value is the answer
}

int main() {
    // (3 + 4) * 5 - 6  written in postfix:
    std::vector<std::string> expr = {"3", "4", "+", "5", "*", "6", "-"};
    std::cout << "3 4 + 5 * 6 -  =  " << evalRPN(expr) << '\n';   // 29
}
