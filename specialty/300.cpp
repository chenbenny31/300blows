// LC 300 Longest Increasing Subsequence: length of the longest strictly increasing
// subsequence of nums.
// Trick: tails[k] = smallest tail of a length-(k+1) run; binary-search replace in tails
// Recognize: LIS in O(n log n), or anything reducible to LIS (envelopes, stacking)
// Follow-up: the O(n^2) dp version.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::max;

// Patience tails + hand-written lower bound. T: O(n log n), S: O(n).
int lengthOfLIS(const vector<int>& nums) {
    // tails[k] = smallest tail of any increasing subsequence of length k + 1
    vector<int> tails;
    for (int x : nums) {
        // lower bound: first tail >= x (>= keeps it strictly increasing)
        int lo = 0, hi = tails.size();
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (tails[mid] < x) {
                lo = mid + 1;
            } else {
                hi = mid;
            }
        }
        if (lo == (int)tails.size()) {
            tails.push_back(x);
        } else {
            tails[lo] = x;   // same length, smaller tail
        }
    }
    return tails.size();
}

// follow-up: dp over every earlier element. T: O(n^2), S: O(n).
int lengthOfLISQuadratic(const vector<int>& nums) {
    int n = nums.size();
    vector<int> dp(n, 1);   // dp[i] = LIS length ending at nums[i]
    int best = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (nums[j] < nums[i]) { dp[i] = max(dp[i], dp[j] + 1); }
        }
        best = max(best, dp[i]);
    }
    return best;
}

void test(const string& label, const vector<int>& nums, int want) {
    cout << label << (lengthOfLIS(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {10, 9, 2, 5, 3, 7, 101, 18}, 4);
    test("lc ex2", {0, 1, 0, 3, 2, 3}, 4);
    test("all equal", {7, 7, 7}, 1);
}
