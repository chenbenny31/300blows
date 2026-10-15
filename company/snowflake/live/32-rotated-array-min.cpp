// B18 / LC 153: minimum of a rotated strictly increasing array in O(log n); follow-up LC 154 allows duplicates.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <functional>
#include <climits>

using std::cout;
using std::string;
using std::vector;

class RotatedMinFinder {
public:
    // binary search comparing mid with hi, T: O(log n), S: O(1)
    int findMin(const vector<int>& nums) {
        int lo = 0;
        int hi = nums.size() - 1;
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (nums[mid] > nums[hi]) {
                lo = mid + 1;   // drop is strictly right of mid
            } else {
                hi = mid;       // mid itself may be the min, keep it
            }
        }
        return nums[lo];
    }

    // same search, on a tie shrink hi by one, T: O(log n) average, O(n) worst (all equal), S: O(1)
    int findMin2(const vector<int>& nums) {
        int lo = 0;
        int hi = nums.size() - 1;
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (nums[mid] > nums[hi]) {
                lo = mid + 1;
            } else if (nums[mid] < nums[hi]) {
                hi = mid;
            } else {
                hi--;           // nums[hi] has an equal copy at mid, so dropping hi keeps the min value
            }
        }
        return nums[lo];
    }
};

bool test(const vector<int>& nums, int expected) {
    RotatedMinFinder rf;
    int got = rf.findMin(nums);
    int got2 = rf.findMin2(nums);
    if (got == expected && got2 == expected) { return true; }
    cout << "FAIL size " << nums.size() << ": got " << got << " / " << got2 << ", expected " << expected << "\n";
    return false;
}

bool testDup(const string& label, const vector<int>& nums, int expected) {
    RotatedMinFinder rf;
    int got = rf.findMin2(nums);
    if (got == expected) { return true; }
    cout << "FAIL dup " << label << ": got " << got << ", expected " << expected << "\n";
    return false;
}

int main() {
    bool ok = true;
    ok &= test({3, 4, 5, 1, 2}, 1);                  // LC 153 example 1
    ok &= test({4, 5, 6, 7, 0, 1, 2}, 0);            // LC 153 example 2
    ok &= test({11, 13, 15, 17}, 11);                // not rotated
    ok &= test({1}, 1);                              // single element
    ok &= test({2, 1}, 1);                           // two elements, rotated
    ok &= test({2, 3, 4, 5, 1}, 1);                  // min at last index
    ok &= test({INT_MAX, INT_MIN}, INT_MIN);         // extreme values, mid without overflow
    ok &= testDup("lc154-1", {1, 3, 5}, 1);          // LC 154 example 1
    ok &= testDup("lc154-2", {2, 2, 2, 0, 1}, 0);    // LC 154 example 2
    ok &= testDup("tie-left", {3, 3, 1, 3}, 1);      // mid == hi, min on the left
    ok &= testDup("tie-right", {10, 1, 10, 10, 10}, 1);  // mid == hi, min left of mid
    ok &= testDup("all-equal", {7, 7, 7, 7}, 7);     // worst case O(n)
    cout << (ok ? "all tests passed\n" : "some tests FAILED\n");
    return ok ? 0 : 1;
}
