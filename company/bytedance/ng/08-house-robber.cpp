// ByteDance (hack2hire) / LC 198: houses in a row hold nums[i]; max total without
// robbing two adjacent houses.
// Follow-up LC 213: the houses form a circle, so the first and last are adjacent.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::max;

// dp with two rolling values. T: O(n), S: O(1).
int rob(const vector<int>& nums) {
    int prev = 0;   // best loot up to house i-2
    int cur = 0;    // best loot up to house i-1
    for (int x : nums) {
        int best = max(cur, prev + x);   // skip x, or take x on top of i-2
        prev = cur;
        cur = best;
    }
    return cur;
}

// follow-up: first and last can't both be robbed, so drop one end or the other.
// T: O(n), S: O(n).
int robCircle(const vector<int>& nums) {
    int n = nums.size();
    if (n == 1) { return nums[0]; }
    vector<int> noLast(nums.begin(), nums.end() - 1);
    vector<int> noFirst(nums.begin() + 1, nums.end());
    return max(rob(noLast), rob(noFirst));
}

void test(const string& label, const vector<int>& nums, int want) {
    cout << label << (rob(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {1, 2, 3, 1}, 4);
    test("lc ex2", {2, 7, 9, 3, 1}, 12);
    test("skip two", {2, 1, 1, 2}, 4);
}
