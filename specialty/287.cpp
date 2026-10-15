// LC 287 Find the Duplicate Number: nums has n+1 values in [1, n] and one value repeats
// (possibly many times); find it without modifying nums, in O(1) extra space.
// Trick: i -> nums[i] is a linked list; the duplicate is its cycle entry (Floyd)
// Recognize: values in 1..n usable as indices + read-only array + O(1) space.
// Follow-up: binary search on the value.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Floyd from index 0 (no value points to 0, so 0 is outside the cycle); the entry has
// two incoming edges, i.e. two indices hold the same value. T: O(n), S: O(1).
int findDuplicate(const vector<int>& nums) {
    int slow = 0, fast = 0;
    do {
        slow = nums[slow];
        fast = nums[nums[fast]];
    } while (slow != fast);
    // start-to-entry distance = meet-to-entry distance (mod cycle length)
    slow = 0;
    while (slow != fast) {
        slow = nums[slow];
        fast = nums[fast];
    }
    return slow;
}

// follow-up: binary search on the value, counting values <= mid. T: O(n log n), S: O(1).
int findDuplicateBinary(const vector<int>& nums) {
    int lo = 1, hi = (int)nums.size() - 1;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        int cnt = 0;   // how many values are in [1, mid]
        for (int x : nums) {
            if (x <= mid) { cnt++; }
        }
        // pigeonhole: [1, mid] holds more than mid values only if the duplicate is there
        if (cnt > mid) { hi = mid; } else { lo = mid + 1; }
    }
    return lo;
}

void test(const string& label, const vector<int>& nums, int want) {
    cout << label << (findDuplicate(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {1, 3, 4, 2, 2}, 2);
    test("lc ex2", {3, 1, 3, 4, 2}, 3);
    test("all same", {3, 3, 3, 3}, 3);
}
