// ByteDance (hack2hire) / LC 678: s has '(', ')' and '*'; '*' may be '(', ')' or empty.
// Is s valid?
// Follow-up: list every distinct valid string made by replacing each '*', sorted.
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::set;
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

// cur = s[0..i) after replacements, open = unmatched '(' in cur
void dfs(const string& s, int i, int open, string& cur, set<string>& found) {
    if (open < 0) { return; }   // a ')' with nothing to match: no fix later
    if (i == (int)s.size()) {
        if (open == 0) { found.insert(cur); }
        return;
    }
    if (s[i] != '*') {
        cur.push_back(s[i]);
        dfs(s, i + 1, open + (s[i] == '(' ? 1 : -1), cur, found);
        cur.pop_back();
        return;
    }
    cur.push_back('(');
    dfs(s, i + 1, open + 1, cur, found);
    cur.back() = ')';
    dfs(s, i + 1, open - 1, cur, found);
    cur.pop_back();
    dfs(s, i + 1, open, cur, found);   // '*' as empty
}

// follow-up: DFS over each '*' choice. T: O(3^k * n), k = number of '*'. S: O(3^k * n).
vector<string> allValidStrings(const string& s) {
    set<string> found;   // distinct valid strings, kept sorted
    string cur;
    dfs(s, 0, 0, cur, found);
    return vector<string>(found.begin(), found.end());
}

void test(const string& label, const string& s, bool want) {
    cout << label << (checkValidString(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("plain", "()", true);
    test("star as empty", "(*)", true);
    test("star as open", "(*))", true);
    test("unclosed", "((*", false);
    test("close first", ")(", false);
}
