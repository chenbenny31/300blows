// LC 32 Longest Valid Parentheses: s has only '(' and ')'; return the length of the
// longest well-formed substring.
// Trick: index stack seeded with -1 as the run's base; on ')' pop, then len = i - top
// Recognize: longest balanced substring (not subsequence) of brackets.
// Follow-up: O(1) extra space.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::max;

// Stack of indices; the bottom is the index just before the current valid run.
// T: O(n), S: O(n).
int longestValidParentheses(const string& s) {
    // bottom = last unmatched ')' (or -1); above it = indices of unmatched '('
    vector<int> stk = {-1};
    int n = s.size(), best = 0;
    for (int i = 0; i < n; i++) {
        if (s[i] == '(') {
            stk.push_back(i);
            continue;
        }
        stk.pop_back();   // match the top '(', or discard the old base
        if (stk.empty()) {
            stk.push_back(i);   // unmatched ')': nothing valid can cross it
        } else {
            best = max(best, i - stk.back());
        }
    }
    return best;
}

// follow-up: count '(' and ')' left to right, then right to left. T: O(n), S: O(1).
int longestValidCounter(const string& s) {
    int n = s.size(), best = 0;
    int open = 0, close = 0;
    for (int i = 0; i < n; i++) {
        if (s[i] == '(') { open++; } else { close++; }
        if (open == close) { best = max(best, 2 * close); }
        // an extra ')' can never be matched: restart after it
        if (close > open) { open = close = 0; }
    }
    // the reverse pass catches runs like "(()" where open never comes down to close
    open = close = 0;
    for (int i = n - 1; i >= 0; i--) {
        if (s[i] == '(') { open++; } else { close++; }
        if (open == close) { best = max(best, 2 * open); }
        if (open > close) { open = close = 0; }
    }
    return best;
}

void test(const string& label, const string& s, int want) {
    cout << label << (longestValidParentheses(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", "(()", 2);
    test("lc ex2", ")()())", 4);
    test("empty", "", 0);
    test("nested", "()(())", 6);
}
