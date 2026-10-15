// ByteDance (hack2hire) / LC 3: length of the longest substring of s without
// repeating characters.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::max;

// Sliding window; on a repeat, jump left past the char's last index. T: O(n), S: O(1).
int lengthOfLongestSubstring(const string& s) {
    vector<int> last(256, -1);   // last[c] = latest index of char c, -1 = not seen
    int best = 0, left = 0;      // window = s[left..i]
    int n = s.size();
    for (int i = 0; i < n; i++) {
        unsigned char c = s[i];
        // max: an old index left of the window must not move left backwards
        left = max(left, last[c] + 1);
        last[c] = i;
        best = max(best, i - left + 1);
    }
    return best;
}

void test(const string& label, const string& s, int want) {
    cout << label << (lengthOfLongestSubstring(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", "abcabcbb", 3);
    test("all same", "bbbbb", 1);
    test("stale index", "abba", 2);
    test("empty", "", 0);
}
