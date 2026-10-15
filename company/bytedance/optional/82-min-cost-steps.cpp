// ByteDance (hack2hire): start at index 0 and pay nums[0]; each move goes 1 or 2 steps
// right and pays the cost of the landing index. Min total cost to reach the last index.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::min;

// dp[i] = nums[i] + min(dp[i-1], dp[i-2]) with two rolling values. T: O(n), S: O(1).
int minCost(const vector<int>& nums) {
    // dp[-1] := dp[0], so index 1 can only come from index 0
    int prev = nums[0];   // dp[i-2]
    int cur = nums[0];    // dp[i-1]
    int n = nums.size();
    for (int i = 1; i < n; i++) {
        int best = nums[i] + min(cur, prev);
        prev = cur;
        cur = best;
    }
    return cur;
}

void test(const string& label, const vector<int>& nums, int want) {
    cout << label << (minCost(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("skip middle", {10, 15, 20}, 30);
    test("dodge 100s", {1, 100, 1, 1, 1, 100, 1, 1, 100, 1}, 6);
    test("single", {5}, 5);
}
