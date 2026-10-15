// B17 · LC 438: every start index in s of an anagram of p (lowercase a-z). Follow-up: O(n) without the 26 factor.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// fixed window of size m, compare 26 counts each step, T: O(26n + m), S: O(1)
vector<int> findAnagrams(const string& s, const string& p) {
    int n = s.size(), m = p.size();
    vector<int> starts;
    if (m == 0 || m > n) { return starts; }
    vector<int> need(26, 0);     // letter -> count in p
    vector<int> window(26, 0);   // letter -> count in s[i-m+1..i]
    for (char c : p) { need[c - 'a']++; }
    for (int i = 0; i < n; i++) {
        window[s[i] - 'a']++;
        if (i >= m) { window[s[i - m] - 'a']--; }
        if (i >= m - 1 && window == need) { starts.push_back(i - m + 1); }
    }
    return starts;
}

// follow-up: one owed array + count of p letters still missing, T: O(n + m), S: O(1)
vector<int> findAnagrams2(const string& s, const string& p) {
    int n = s.size(), m = p.size();
    vector<int> starts;
    if (m == 0 || m > n) { return starts; }
    vector<int> owed(26, 0);   // letter -> count in p minus count in window (negative = surplus)
    for (char c : p) { owed[c - 'a']++; }
    int missing = m;           // p letters not yet covered by the window
    for (int i = 0; i < n; i++) {
        if (owed[s[i] - 'a'] > 0) { missing--; }   // check before decrement: only a needed copy counts
        owed[s[i] - 'a']--;
        if (i >= m) {
            int out = s[i - m] - 'a';
            owed[out]++;
            if (owed[out] > 0) { missing++; }      // check after increment: we now lack this letter
        }
        if (missing == 0) { starts.push_back(i - m + 1); }
    }
    return starts;
}

void test(const string& label, const string& s, const string& p, const vector<int>& expect) {
    vector<int> got = findAnagrams(s, p);
    cout << (got == expect ? "PASS " : "FAIL ") << label << "\n";
}

void test2(const string& label, const string& s, const string& p, const vector<int>& expect) {
    vector<int> got = findAnagrams2(s, p);
    cout << (got == expect ? "PASS " : "FAIL ") << label << "\n";
}

int main() {
    test("lc1", "cbaebabacd", "abc", {0, 6});   // target example
    test("overlap", "abab", "ab", {0, 1, 2});   // overlapping matches
    test("p longer", "ab", "abc", {});          // m > n guard
    test("repeats", "aaaa", "aa", {0, 1, 2});   // duplicate letters counted

    test2("lc1", "cbaebabacd", "abc", {0, 6});  // target example
    test2("empty p", "abc", "", {});            // empty pattern
    test2("surplus", "aab", "ab", {1});         // surplus copy must not cover
    test2("near miss", "abaacbabc", "abc", {3, 4, 6});   // missing goes up and down
}
