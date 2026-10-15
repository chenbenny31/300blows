// ByteDance (hack2hire) / LC 5: return the longest palindromic substring of s.
#include <iostream>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::max;

// grow s[l..r] outward while it stays a palindrome; returns the final length
int expand(const string& s, int l, int r) {
    int n = s.size();
    while (l >= 0 && r < n && s[l] == s[r]) {
        l--;
        r++;
    }
    return r - l - 1;
}

// Expand around every center: a char (odd) and a gap (even). T: O(n^2), S: O(1).
string longestPalindrome(const string& s) {
    int start = 0, best = 0;   // best palindrome = s.substr(start, best)
    int n = s.size();
    for (int i = 0; i < n; i++) {
        int len = max(expand(s, i, i), expand(s, i, i + 1));
        if (len > best) {
            best = len;
            // works for both: odd centers on i, even on the gap after i
            start = i - (len - 1) / 2;
        }
    }
    return s.substr(start, best);
}

void test(const string& label, const string& s, const string& want) {
    cout << label << (longestPalindrome(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("odd", "cabad", "aba");
    test("even", "cbbd", "bb");
    test("single", "a", "a");
}
