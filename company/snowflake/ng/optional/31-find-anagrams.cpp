// B17 · LC 438: every start index in s of a substring that is an anagram of p
// (lowercase a-z only).
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Fixed window of size m; compare 26 counts at each step. T: O(26n + m), S: O(1).
vector<int> findAnagrams(const string& s, const string& p) {
    int n = s.size(), m = p.size();
    vector<int> starts;
    if (m == 0 || m > n) { return starts; }
    vector<int> need(26);     // need[k] = count of letter k in p
    vector<int> window(26);   // window[k] = count of letter k in s[i-m+1..i]
    for (char c : p) { need[c - 'a']++; }
    for (int i = 0; i < n; i++) {
        window[s[i] - 'a']++;
        // drop the char that just slid out on the left
        if (i >= m) { window[s[i - m] - 'a']--; }
        if (i >= m - 1 && window == need) { starts.push_back(i - m + 1); }
    }
    return starts;
}

void test(const string& label, const string& s, const string& p,
          const vector<int>& want) {
    cout << label << (findAnagrams(s, p) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("two matches", "cbaebabacd", "abc", {0, 6});
    test("overlapping", "abab", "ab", {0, 1, 2});
    test("p longer", "ab", "abc", {});
}
