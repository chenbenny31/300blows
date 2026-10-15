// ByteDance / LC 540: in a sorted array every element appears exactly twice except one,
// which appears once. Return it in O(log n) time and O(1) space.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Binary search on even indices. T: O(log n), S: O(1).
int singleNonDuplicate(const vector<int>& nums) {
    int lo = 0, hi = nums.size() - 1;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        mid -= mid % 2;   // even index: before the single, pairs start at even indices
        if (nums[mid] == nums[mid + 1]) {
            lo = mid + 2;   // pair intact: the single is to the right
        } else {
            hi = mid;       // pair broken: the single is mid or to the left
        }
    }
    return nums[lo];
}

void test(const string& label, const vector<int>& nums, int want) {
    cout << label << (singleNonDuplicate(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {1, 1, 2, 3, 3, 4, 4, 8, 8}, 2);
    test("lc ex2", {3, 3, 7, 7, 10, 11, 11}, 10);
    test("single", {1}, 1);
}
