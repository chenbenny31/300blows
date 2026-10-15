// B17 / LC 438: return every start index in s of a substring that is an anagram of p (lowercase a-z).

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <functional>
#include <climits>

using std::cout;
using std::string;
using std::vector;

class AnagramFinder {
public:
    // fixed window of size m, compare 26 counts each step, T: O(26 n), S: O(1)
    vector<int> findAnagrams(const string& s, const string& p) {
        int n = s.size();
        int m = p.size();
        vector<int> starts;
        if (m == 0 || m > n) { return starts; }
        vector<int> need(26, 0);     // letter -> count in p
        vector<int> window(26, 0);   // letter -> count in s[i-m+1..i]
        for (char c : p) { need[c - 'a']++; }
        for (int i = 0; i < n; i++) {
            window[s[i] - 'a']++;
            if (i >= m) { window[s[i - m] - 'a']--; }   // drop the char that just left the window
            if (i >= m - 1 && window == need) { starts.push_back(i - m + 1); }
        }
        return starts;
    }

    // one count array plus number of p-letters still missing, T: O(n), S: O(1)
    vector<int> findAnagrams2(const string& s, const string& p) {
        int n = s.size();
        int m = p.size();
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
};

bool test(const string& s, const string& p, const vector<int>& expected) {
    AnagramFinder af;
    vector<int> got = af.findAnagrams(s, p);
    vector<int> got2 = af.findAnagrams2(s, p);
    if (got == expected && got2 == expected) { return true; }
    cout << "FAIL s=\"" << s << "\" p=\"" << p << "\"\n";
    return false;
}

int main() {
    bool ok = true;
    ok &= test("cbaebabacd", "abc", {0, 6});      // LC example 1
    ok &= test("abab", "ab", {0, 1, 2});          // LC example 2: overlapping matches
    ok &= test("a", "a", {0});                    // smallest match, window == whole s
    ok &= test("abc", "", {});                    // empty pattern -> empty by contract
    ok &= test("ab", "abc", {});                  // p longer than s
    ok &= test("", "", {});                       // both empty
    ok &= test("aaaa", "aa", {0, 1, 2});          // repeated letters
    ok &= test("aab", "ab", {1});                 // surplus copy must not count as covering
    ok &= test("abaacbabc", "abc", {3, 4, 6});    // matches after several near misses
    ok &= test("xyz", "zyx", {0});                // match ends at last index
    cout << (ok ? "all tests passed\n" : "some tests FAILED\n");
    return ok ? 0 : 1;
}
