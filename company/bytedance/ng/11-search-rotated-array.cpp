// ByteDance (hack2hire) / LC 33: nums is an ascending array of distinct values rotated
// at an unknown pivot; return the index of target, or -1. O(log n).
// Follow-up LC 81: nums may contain duplicates; return whether target exists.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Binary search; one half of [lo, hi] is always sorted. T: O(log n), S: O(1).
int search(const vector<int>& nums, int target) {
    int lo = 0, hi = (int)nums.size() - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (nums[mid] == target) { return mid; }
        if (nums[lo] <= nums[mid]) {
            // left half sorted: keep it only if target lies inside its range
            if (nums[lo] <= target && target < nums[mid]) {
                hi = mid - 1;
            } else {
                lo = mid + 1;
            }
        } else {
            // right half sorted
            if (nums[mid] < target && target <= nums[hi]) {
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }
    }
    return -1;
}

// follow-up: same, but when lo, mid, hi are equal neither half is known sorted,
// so shrink both ends. T: O(n) worst (all equal), O(log n) typical, S: O(1).
bool searchDup(const vector<int>& nums, int target) {
    int lo = 0, hi = (int)nums.size() - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (nums[mid] == target) { return true; }
        if (nums[lo] == nums[mid] && nums[mid] == nums[hi]) {
            lo++;
            hi--;
        } else if (nums[lo] <= nums[mid]) {
            if (nums[lo] <= target && target < nums[mid]) {
                hi = mid - 1;
            } else {
                lo = mid + 1;
            }
        } else {
            if (nums[mid] < target && target <= nums[hi]) {
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }
    }
    return false;
}

void test(const string& label, const vector<int>& nums, int target, int want) {
    cout << label << (search(nums, target) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {4, 5, 6, 7, 0, 1, 2}, 0, 4);
    test("missing", {4, 5, 6, 7, 0, 1, 2}, 3, -1);
    test("single", {1}, 1, 0);
}
