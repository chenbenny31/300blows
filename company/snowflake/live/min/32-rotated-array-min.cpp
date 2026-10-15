// B18 · LC 153: minimum of a rotated strictly increasing array in O(log n). Follow-up LC 154: duplicates allowed.
#include <iostream>
#include <vector>
#include <climits>
using std::cout;
using std::vector;

// binary search comparing mid with hi, T: O(log n), S: O(1)
int findMin(const vector<int>& nums) {
    int lo = 0, hi = nums.size() - 1;
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

// follow-up: same search, on a tie shrink hi by one, T: O(log n) average, O(n) worst, S: O(1)
int findMin2(const vector<int>& nums) {
    int lo = 0, hi = nums.size() - 1;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (nums[mid] > nums[hi]) {
            lo = mid + 1;
        } else if (nums[mid] < nums[hi]) {
            hi = mid;
        } else {
            hi--;           // nums[hi] has an equal copy at mid, dropping hi keeps the min value
        }
    }
    return nums[lo];
}

void test(const vector<int>& nums, int expect) {
    int got = findMin(nums);
    cout << (got == expect ? "PASS " : "FAIL ") << got << " expect " << expect << "\n";
}

void test2(const vector<int>& nums, int expect) {
    int got = findMin2(nums);
    cout << (got == expect ? "PASS " : "FAIL ") << got << " expect " << expect << "\n";
}

int main() {
    test({4, 5, 6, 7, 0, 1, 2}, 0);       // target example
    test({11, 13, 15, 17}, 11);           // not rotated
    test({1}, 1);                         // single element
    test({INT_MAX, INT_MIN}, INT_MIN);    // extreme values, mid without overflow

    test2({2, 2, 2, 0, 1}, 0);            // LC 154 example
    test2({3, 3, 1, 3}, 1);               // mid == hi, min on the left
    test2({10, 1, 10, 10, 10}, 1);        // mid == hi, min left of mid
    test2({7, 7, 7, 7}, 7);               // all equal, worst case
}
