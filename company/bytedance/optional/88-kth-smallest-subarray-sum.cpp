// ByteDance (hack2hire) / LC 1918: nums are positive; return the k-th smallest sum
// among all n * (n + 1) / 2 non-empty subarrays (1-indexed k).
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// number of subarrays with sum <= s; positive nums make the window shrink monotonic
long long countAtMost(const vector<int>& nums, int s) {
    int n = nums.size();
    long long count = 0;
    int sum = 0;   // sum of nums[left..right]
    int left = 0;
    for (int right = 0; right < n; right++) {
        sum += nums[right];
        while (sum > s) { sum -= nums[left++]; }
        count += right - left + 1;   // every subarray ending at right, starting >= left
    }
    return count;
}

// Binary search the smallest s with countAtMost(s) >= k.
// T: O(n log W), S: O(1); W = sum(nums).
int kthSmallestSubarraySum(const vector<int>& nums, int k) {
    int lo = 1, hi = 0;
    for (int x : nums) { hi += x; }
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        // enough sums <= mid: the answer is mid or smaller
        if (countAtMost(nums, mid) >= k) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    return lo;
}

void test(const string& label, const vector<int>& nums, int k, int want) {
    cout << label << (kthSmallestSubarraySum(nums, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {2, 1, 3}, 4, 3);
    test("lc ex2", {3, 3, 5, 5}, 7, 10);
}
