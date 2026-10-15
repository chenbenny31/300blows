// LC 678: s has '(', ')' and '*'; '*' may be '(', ')' or empty. Is s valid?
#include <iostream>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::max;

// Greedy: track the range [lo, hi] of possible open counts. T: O(n), S: O(1).
bool checkValidString(const string& s) {
    int lo = 0;   // fewest opens possible (every '*' used as ')' when allowed)
    int hi = 0;   // most opens possible (every '*' used as '(')
    for (char c : s) {
        if (c == '(') {
            lo++;
            hi++;
        } else if (c == ')') {
            lo--;
            hi--;
        } else {
            lo--;
            hi++;
        }
        if (hi < 0) { return false; }   // too many ')' even with every '*' as '('
        lo = max(lo, 0);                // open count can never go negative
    }
    return lo == 0;
}

void test(const string& label, const string& s, bool want) {
    cout << label << (checkValidString(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("star as close", "(*))", true);
    test("star as empty", "(*)", true);
    test("close first", ")(", false);
    test("unclosed", "((*", false);
}
