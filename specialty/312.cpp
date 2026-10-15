// LC 312 Burst Balloons: bursting balloon i earns nums[left] * nums[i] * nums[right] with
// its current neighbors (out of range = 1). Return the max total coins.
// Trick: pick the LAST balloon k burst in (i, j); its neighbors are then fixed at i and j
// Recognize: removing an item changes neighbors -> interval DP over the last one removed
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::max;

// Interval DP by gap length on the 1-padded array. T: O(n^3), S: O(n^2).
int maxCoins(const vector<int>& nums) {
    vector<int> v = {1};   // nums with a 1 on each side
    v.insert(v.end(), nums.begin(), nums.end());
    v.push_back(1);
    int m = v.size();
    // dp[i][j] = max coins from bursting everything strictly between i and j
    vector<vector<int>> dp(m, vector<int>(m, 0));
    for (int len = 2; len < m; len++) {
        for (int i = 0; i + len < m; i++) {
            int j = i + len;
            for (int k = i + 1; k < j; k++) {
                // k goes last, so (i, k) and (k, j) are independent subproblems
                dp[i][j] = max(dp[i][j], dp[i][k] + dp[k][j] + v[i] * v[k] * v[j]);
            }
        }
    }
    return dp[0][m - 1];
}

void test(const string& label, const vector<int>& nums, int want) {
    cout << label << (maxCoins(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {3, 1, 5, 8}, 167);
    test("lc ex2", {1, 5}, 10);
}
