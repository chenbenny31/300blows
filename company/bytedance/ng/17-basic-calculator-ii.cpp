// ByteDance (hack2hire) / LC 227: evaluate a valid string of non-negative integers,
// + - * / and spaces; integer division truncates toward zero. No parentheses.
// Follow-up: O(1) extra space.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// One pass; push each term with its sign, apply * and / to the last term at once.
// T: O(n), S: O(n).
int calculate(const string& s) {
    vector<int> terms;   // signed terms whose sum is the answer
    int num = 0;
    char op = '+';       // operator before num
    int n = s.size();
    for (int i = 0; i < n; i++) {
        char c = s[i];
        bool digit = c >= '0' && c <= '9';
        if (digit) { num = num * 10 + (c - '0'); }
        // apply the pending op at each operator, and once more at the end
        if ((!digit && c != ' ') || i == n - 1) {
            if (op == '+') {
                terms.push_back(num);
            } else if (op == '-') {
                terms.push_back(-num);
            } else if (op == '*') {
                terms.back() *= num;
            } else {
                terms.back() /= num;
            }
            op = c;
            num = 0;
        }
    }
    int result = 0;
    for (int t : terms) { result += t; }
    return result;
}

// follow-up: keep only the last term instead of the stack. T: O(n), S: O(1).
int calculateO1(const string& s) {
    int result = 0;   // sum of finished terms
    int last = 0;     // current signed term, still open to * and /
    int num = 0;
    char op = '+';
    int n = s.size();
    for (int i = 0; i < n; i++) {
        char c = s[i];
        bool digit = c >= '0' && c <= '9';
        if (digit) { num = num * 10 + (c - '0'); }
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
    test("lc ex1", "3+2*2", 7);
    test("lc ex2", " 3/2 ", 1);
    test("lc ex3", " 3+5 / 2 ", 5);
}
