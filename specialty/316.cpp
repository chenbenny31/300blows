// LC 316 Remove Duplicate Letters: remove duplicate letters from s (lowercase) so each
// letter appears once; return the lexicographically smallest such subsequence.
// Trick: increasing stack; pop a larger top only if it appears again later (last index)
// Recognize: smallest subsequence using each distinct char once (same as LC 1081).
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Monotonic stack; drop a larger top if it appears again later. T: O(n), S: O(1).
string removeDuplicateLetters(const string& s) {
    int n = s.size();
    vector<int> last(26, 0);        // last[c] = last index of letter c in s
    for (int i = 0; i < n; i++) { last[s[i] - 'a'] = i; }
    vector<bool> seen(26, false);   // seen[c] = letter c is in stk
    string stk;                     // result so far, as small as possible
    for (int i = 0; i < n; i++) {
        char c = s[i];
        if (seen[c - 'a']) { continue; }
        // a larger top that reappears later can be re-added after c
        while (!stk.empty() && stk.back() > c && last[stk.back() - 'a'] > i) {
            seen[stk.back() - 'a'] = false;
            stk.pop_back();
        }
        stk += c;
        seen[c - 'a'] = true;
    }
    return stk;
}

void test(const string& label, const string& s, const string& want) {
    cout << label << (removeDuplicateLetters(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", "bcabc", "abc");
    test("lc ex2", "cbacdcbc", "acdb");
}
