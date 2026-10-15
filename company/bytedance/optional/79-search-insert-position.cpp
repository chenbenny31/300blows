// ByteDance (hack2hire) / LC 35: nums is sorted with distinct values; return the index
// of target, or the index where it would be inserted to keep order. O(log n).
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Lower bound: first index with nums[i] >= target. T: O(log n), S: O(1).
int searchInsert(const vector<int>& nums, int target) {
    int lo = 0, hi = nums.size();   // answer lies in [lo, hi]; hi = n means append
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (nums[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid;   // mid may be the answer, keep it
        }
    }
    return lo;
}

void test(const string& label, const vector<int>& nums, int target, int want) {
    cout << label << (searchInsert(nums, target) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("found", {1, 3, 5, 6}, 5, 2);
    test("middle", {1, 3, 5, 6}, 2, 1);
    test("past end", {1, 3, 5, 6}, 7, 4);
    test("before start", {1, 3, 5, 6}, 0, 0);
}
