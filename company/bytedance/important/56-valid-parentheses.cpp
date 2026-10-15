// ByteDance (hack2hire) / LC 20: s holds only '(', ')', '[', ']', '{', '}'; return true
// if every bracket is closed by the same type in the correct order.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Stack of open brackets; a closer must match the top. T: O(n), S: O(n).
bool isValid(const string& s) {
    vector<char> stack;   // unmatched open brackets, innermost at the back
    for (char c : s) {
        if (c == '(' || c == '[' || c == '{') {
            stack.push_back(c);
            continue;
        }
        char open = c == ')' ? '(' : c == ']' ? '[' : '{';
        if (stack.empty() || stack.back() != open) { return false; }
        stack.pop_back();
    }
    // leftover openers were never closed
    return stack.empty();
}

void test(const string& label, const string& s, bool want) {
    cout << label << (isValid(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("pair", "()", true);
    test("all types", "()[]{}", true);
    test("wrong type", "(]", false);
    test("wrong order", "([)]", false);
}
