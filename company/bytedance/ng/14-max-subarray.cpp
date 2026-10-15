// ByteDance (hack2hire) / LC 53: return the largest sum of a non-empty contiguous
// subarray of nums.
// Follow-up: also return the subarray's [start, end] indices.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::max;
using std::pair;

// Kadane: best sum ending at i extends the one ending at i-1 or restarts.
// T: O(n), S: O(1).
int maxSubArray(const vector<int>& nums) {
    int cur = nums[0];    // best sum of a subarray ending at i
    int best = nums[0];   // best sum seen anywhere
    int n = nums.size();
    for (int i = 1; i < n; i++) {
        cur = max(nums[i], cur + nums[i]);   // restart when the prefix hurts
        best = max(best, cur);
    }
    return best;
}

// follow-up: Kadane, remembering where the current run started. T: O(n), S: O(1).
pair<int, int> maxSubArrayBounds(const vector<int>& nums) {
    int cur = nums[0], best = nums[0];
    int start = 0;                // start index of the run ending at i
    int bestL = 0, bestR = 0;     // bounds of the best run
    int n = nums.size();
    for (int i = 1; i < n; i++) {
        if (cur < 0) {
            cur = nums[i];
            start = i;
        } else {
            cur += nums[i];
        }
        if (cur > best) {
            best = cur;
            bestL = start;
            bestR = i;
        }
    }
    return {bestL, bestR};
}

void test(const string& label, const vector<int>& nums, int want) {
    cout << label << (maxSubArray(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {-2, 1, -3, 4, -1, 2, 1, -5, 4}, 6);
    test("single", {1}, 1);
    test("all negative", {-3, -1, -2}, -1);
}
