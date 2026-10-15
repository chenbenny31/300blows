// LC 34: nums is sorted ascending; return [first, last] index of target, or [-1, -1].
// Must run in O(log n).
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// first index with nums[i] >= target (n if none). T: O(log n), S: O(1).
int lowerBound(const vector<int>& nums, int target) {
    int lo = 0, hi = nums.size();   // answer lies in [lo, hi]
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (nums[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;
}

// Two lower bounds: target and target + 1. T: O(log n), S: O(1).
vector<int> searchRange(const vector<int>& nums, int target) {
    int first = lowerBound(nums, target);
    if (first == (int)nums.size() || nums[first] != target) { return {-1, -1}; }
    // last target sits just before the first value > target
    return {first, lowerBound(nums, target + 1) - 1};
}

void test(const string& label, const vector<int>& nums, int target,
          const vector<int>& want) {
    cout << label << (searchRange(nums, target) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {5, 7, 7, 8, 8, 10}, 8, {3, 4});
    test("missing", {5, 7, 7, 8, 8, 10}, 6, {-1, -1});
    test("all same", {2, 2}, 2, {0, 1});
    test("empty", {}, 0, {-1, -1});
}
