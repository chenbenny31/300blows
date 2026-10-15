// LC 224: evaluate a valid expression of non-negative integers, '+', '-', '(', ')' and
// spaces. '-' may be unary, e.g. "-(2+3)"; no eval().
// Follow-up LC 227: + - * / with precedence, no parentheses; '/' truncates toward zero.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;
using std::pair;

// One pass; at '(' save (result so far, sign before the paren) and start fresh.
// T: O(n), S: O(n).
int calculate(const string& s) {
    long long result = 0;   // sum of finished terms at the current depth
    long long num = 0;
    int sign = 1;           // sign before num
    // saved[d] = (outer result, sign before '(') for each open paren
    vector<pair<long long, int>> saved;
    for (char c : s) {
        if (c >= '0' && c <= '9') {
            num = num * 10 + (c - '0');
        } else if (c == '+' || c == '-') {
            // unary minus works too: it adds 0 first
            result += sign * num;
            num = 0;
            sign = c == '+' ? 1 : -1;
        } else if (c == '(') {
            saved.push_back({result, sign});
            result = 0;
            sign = 1;
        } else if (c == ')') {
            result += sign * num;
            num = 0;
            // the paren's value becomes one signed term of the outer sum
            result = saved.back().first + saved.back().second * result;
            saved.pop_back();
        }
    }
    return result + sign * num;
}

// follow-up: keep only the last term open to * and /. T: O(n), S: O(1).
int calculateII(const string& s) {
    int result = 0;   // sum of finished terms
    int last = 0;     // current signed term, still open to * and /
    int num = 0;
    char op = '+';    // operator before num
    int n = s.size();
    for (int i = 0; i < n; i++) {
        char c = s[i];
        bool digit = c >= '0' && c <= '9';
        if (digit) { num = num * 10 + (c - '0'); }
        // apply the pending op at each operator, and once more at the end
        if ((!digit && c != ' ') || i == n - 1) {
            if (op == '+' || op == '-') {
                // + or - closes the last term
                result += last;
                last = op == '+' ? num : -num;
            } else if (op == '*') {
                last *= num;
            } else {
                last /= num;
            }
            op = c;
            num = 0;
        }
    }
    return result + last;
}

void test(const string& label, const string& s, int want) {
    cout << label << (calculate(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("spaces", " 2-1 + 2 ", 3);
    test("nested", "(1+(4+5+2)-3)+(6+8)", 23);
    test("unary minus", "-(2+3)", -5);
    test("minus paren", "1-(-(2))", 3);
}
