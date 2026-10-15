// A04 · LC 704: search a sorted int array for target; return its index or -1.
// Follow-up (LC 34 style): with duplicates, return the leftmost matching index.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Closed-interval binary search. T: O(log n), S: O(1).
int search(const vector<int>& nums, int target) {
    int lo = 0, hi = (int)nums.size() - 1;   // answer, if any, is in nums[lo..hi]
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (nums[mid] == target) { return mid; }
        if (nums[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return -1;
}

// follow-up: lower bound (first index with nums[i] >= target), then confirm.
// T: O(log n), S: O(1).
int searchLeftmost(const vector<int>& nums, int target) {
    int n = nums.size();
    int lo = 0, hi = n;   // first index >= target is in [lo, hi]; hi == n means none
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (nums[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid;     // mid may be the first match, keep it
        }
    }
    return lo < n && nums[lo] == target ? lo : -1;
}

void test(const string& label, const vector<int>& nums, int target, int want) {
    cout << label << (search(nums, target) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("found", {-1, 0, 3, 5, 9, 12}, 9, 4);
    test("gap", {-1, 0, 3, 5, 9, 12}, 2, -1);
    test("last index", {-1, 0, 3, 5, 9, 12}, 12, 5);
    test("empty", {}, 5, -1);
}
