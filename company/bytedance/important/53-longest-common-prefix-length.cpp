// ByteDance / LC 3043: positive ints arr1, arr2. Return the length (in digits) of the
// longest common prefix over all pairs (x from arr1, y from arr2); 0 if none.
#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::unordered_set;
using std::max;

// Hash every numeric prefix of arr1; strip digits of each y until one matches.
// T: O((n + m) * D), S: O(n * D); D = max digits (<= 9).
int longestCommonPrefix(const vector<int>& arr1, const vector<int>& arr2) {
    unordered_set<int> prefixes;   // every prefix of every x, e.g. 123 -> 123, 12, 1
    for (int x : arr1) {
        for (; x > 0; x /= 10) { prefixes.insert(x); }
    }
    int best = 0;
    for (int y : arr2) {
        // the first hit while stripping is y's longest matching prefix
        while (y > 0 && !prefixes.count(y)) { y /= 10; }
        int len = 0;
        for (; y > 0; y /= 10) { len++; }
        best = max(best, len);
    }
    return best;
}

void test(const string& label, const vector<int>& arr1, const vector<int>& arr2,
          int want) {
    cout << label << (longestCommonPrefix(arr1, arr2) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {1, 10, 100}, {1000}, 3);
    test("no match", {1, 2, 3}, {4, 4, 4}, 0);
}
