// B18 · LC 153: minimum of a non-empty rotated strictly increasing array in O(log n).
// Follow-up LC 154: duplicates allowed.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Binary search vs nums[hi]; the min always stays in [lo, hi]. T: O(log n), S: O(1).
int findMin(const vector<int>& nums) {
    int lo = 0, hi = nums.size() - 1;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (nums[mid] > nums[hi]) {
            lo = mid + 1;   // the drop is strictly right of mid
        } else {
            hi = mid;       // mid itself may be the min
        }
    }
    return nums[lo];
}

// follow-up: on a tie drop hi; mid holds an equal copy, so the min value survives.
// T: O(log n) average, O(n) worst (all equal), S: O(1).
int findMinDup(const vector<int>& nums) {
    int lo = 0, hi = nums.size() - 1;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (nums[mid] > nums[hi]) {
            lo = mid + 1;
        } else if (nums[mid] < nums[hi]) {
            hi = mid;
        } else {
            hi--;
        }
    }
    return nums[lo];
}

void test(const string& label, const vector<int>& nums, int want) {
    cout << label << (findMin(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("rotated", {3, 4, 5, 1, 2}, 1);
    test("not rotated", {11, 13, 15, 17}, 11);
    test("two", {2, 1}, 1);
    test("single", {1}, 1);
}
