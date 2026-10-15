// LC 713: nums are positive ints; count contiguous subarrays whose product is < k.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Sliding window: shrink until product < k; every subarray ending at right counts.
// T: O(n), S: O(1).
int numSubarrayProductLessThanK(const vector<int>& nums, int k) {
    // products are >= 1, so nothing is < k when k <= 1 (and the window would underflow)
    if (k <= 1) { return 0; }
    int n = nums.size();
    long long prod = 1;   // product of nums[left..right]
    int count = 0, left = 0;
    for (int right = 0; right < n; right++) {
        prod *= nums[right];
        while (prod >= k) { prod /= nums[left++]; }
        count += right - left + 1;
    }
    return count;
}

void test(const string& label, const vector<int>& nums, int k, int want) {
    int got = numSubarrayProductLessThanK(nums, k);
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {10, 5, 2, 6}, 100, 8);
    test("k is 0", {1, 2, 3}, 0, 0);
    test("k is 1", {1, 1}, 1, 0);
}
