// evaluate an integer expression string, no eval()
// LC224: + - ( ) and unary minus, no * /
// LC227: + - * / with precedence, no parentheses, / truncates toward zero

// which operators? parentheses? unary minus (or plus)?
// spaces anywhere? only non-negative integer literals?
// division: truncate toward zero?
// input always valid? result and intermediates fit in int?

#include <string>
#include <stack>
#include <cctype>
#include <cassert>
#include <iostream>

using std::string;

class Calculator {
public:
    // LC224: each number adds sign*num; '(' saves (result, sign) and starts fresh
    // T: O(n), S: O(depth of parentheses)
    long long calculateParens(const string& s) {
        std::stack<long long> saved; // result before '(' then the sign in front of '('
        long long result = 0; // value of the current parenthesis level so far
        long long sign = 1; // sign for the number being read
        long long num = 0;
        for (char c : s) {
            if (std::isdigit(static_cast<unsigned char>(c))) {
                num = num * 10 + (c - '0');
            } else if (c == '+' || c == '-') {
                result += sign * num;
                num = 0;
                sign = (c == '+') ? 1 : -1; // also covers unary minus: result += 0 first
            } else if (c == '(') {
                saved.push(result);
                saved.push(sign);
                result = 0;
                sign = 1;
            } else if (c == ')') {
                result += sign * num;
                num = 0;
                result *= saved.top(); // sign in front of '(' pops first
                saved.pop();
                result += saved.top();
                saved.pop();
            }
        }
        return result + sign * num;
    }

    // LC227: keep the pending term `last`; * and / change it, + and - fold it into result
    // T: O(n), S: O(1)
    long long calculatePrecedence(const string& s) {
        long long result = 0; // sum of finished terms
        long long last = 0; // current term, signed, still open to * and /
        long long num = 0;
        char op = '+'; // operator in front of num
        int n = static_cast<int>(s.size());
        for (int i = 0; i < n; i++) {
            char c = s[i];
            if (std::isdigit(static_cast<unsigned char>(c))) {
                num = num * 10 + (c - '0');
            }
            if ((!std::isdigit(static_cast<unsigned char>(c)) && c != ' ') || i == n - 1) { // operator or end
                if (op == '+') {
                    result += last;
                    last = num;
                } else if (op == '-') {
                    result += last;
                    last = -num;
                } else if (op == '*') {
                    last *= num;
                } else {
                    last /= num; // C++ truncates toward zero, matches LC
                }
                op = c;
                num = 0;
            }
        }
        return result + last;
    }
};

int main() {
    Calculator calc;

    assert(calc.calculateParens("2-(5-6)") == 3); // target: minus in front of parentheses
    assert(calc.calculateParens("-(2+3)") == -5); // edge: unary minus at the start
    assert(calc.calculateParens("1-(-(2))") == 3); // edge: nested unary minus
    assert(calc.calculateParens("7") == 7); // null: single number

    assert(calc.calculatePrecedence("3+2*2") == 7); // target: * before +
    assert(calc.calculatePrecedence("1-7/2") == -2); // edge: division on a negative term truncates toward zero
    assert(calc.calculatePrecedence(" 3/2 ") == 1); // edge: spaces, last char is a space
    assert(calc.calculatePrecedence("42") == 42); // null: single number

    std::cout << "all tests passed\n";
    return 0;
}
