// A04 · LC 704: index of target in a sorted array (any match), -1 if absent. Follow-up (LC 34 style): leftmost match with duplicates.
#include <iostream>
#include <vector>
#include <climits>
using std::cout;
using std::vector;

// closed-interval binary search, T: O(log n), S: O(1)
int search(const vector<int>& nums, int target) {
    int n = nums.size();
    int lo = 0, hi = n - 1;   // answer, if any, is in nums[lo..hi]
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

// follow-up: lower bound (first nums[i] >= target), then confirm, T: O(log n), S: O(1)
int searchLeftmost(const vector<int>& nums, int target) {
    int n = nums.size();
    int lo = 0, hi = n;       // answer in [lo, hi], hi == n means every value < target
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (nums[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid;         // mid may be the first match, keep it
        }
    }
    return (lo < n && nums[lo] == target) ? lo : -1;
}

void test(const vector<int>& nums, int target, int expect) {
    int got = search(nums, target);
    cout << (got == expect ? "PASS " : "FAIL ") << got << " expect " << expect << "\n";
}

void testLeft(const vector<int>& nums, int target, int expect) {
    int got = searchLeftmost(nums, target);
    cout << (got == expect ? "PASS " : "FAIL ") << got << " expect " << expect << "\n";
}

int main() {
    test({-1, 0, 3, 5, 9, 12}, 9, 4);             // LC example 1
    test({-1, 0, 3, 5, 9, 12}, 2, -1);            // LC example 2: gap between values
    test({}, 5, -1);                              // empty: hi = -1
    test({INT_MIN, 0, INT_MAX}, INT_MAX, 2);      // last index, extreme value

    testLeft({1, 2, 2, 2, 2, 3}, 2, 1);           // leftmost of a run, not mid
    testLeft({2, 2, 2, 2, 2}, 2, 0);              // run covers whole array
    testLeft({1, 3}, 4, -1);                      // lower bound lands on n
    testLeft({}, 1, -1);                          // empty
}
