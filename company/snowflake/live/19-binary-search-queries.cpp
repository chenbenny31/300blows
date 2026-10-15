// A04 · LC 704: search a sorted int array for target, return a matching index or -1.
// Follow-up (LC 34 style): with duplicates, return the leftmost matching index.

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

class SortedSearcher {
public:
    // closed-interval binary search, any matching index, T: O(log n), S: O(1)
    int search(const vector<int>& nums, int target) {
        int n = nums.size();
        int lo = 0;
        int hi = n - 1;                    // answer, if any, is in nums[lo..hi]
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

    // lower bound: first index with nums[i] >= target, then confirm, T: O(log n), S: O(1)
    int searchLeftmost(const vector<int>& nums, int target) {
        int n = nums.size();
        int lo = 0;
        int hi = n;                        // answer in [lo, hi], hi == n means every value < target
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (nums[mid] < target) {
                lo = mid + 1;              // mid and everything left of it are too small
            } else {
                hi = mid;                  // mid may be the first match, keep it
            }
        }
        return (lo < n && nums[lo] == target) ? lo : -1;
    }
};

// expected = leftmost index or -1; search may return any matching index
bool test(const string& label, const vector<int>& nums, int target, int expected) {
    SortedSearcher ss;
    int got = ss.search(nums, target);
    int got2 = ss.searchLeftmost(nums, target);
    bool anyOk = expected == -1 ? got == -1 : (got >= 0 && got < (int)nums.size() && nums[got] == target);
    if (anyOk && got2 == expected) { return true; }
    cout << "FAIL " << label << " target " << target << ": got " << got << " / " << got2
         << ", expected " << expected << "\n";
    return false;
}

int main() {
    bool ok = true;
    ok &= test("lc ex1", {-1, 0, 3, 5, 9, 12}, 9, 4);           // LC 704 example 1
    ok &= test("lc ex2", {-1, 0, 3, 5, 9, 12}, 2, -1);          // LC 704 example 2, gap between values
    ok &= test("empty", {}, 5, -1);                             // smallest input
    ok &= test("single hit", {5}, 5, 0);                        // one element, found
    ok &= test("single miss", {5}, -5, -1);                     // one element, absent
    ok &= test("below range", {1, 3}, 0, -1);                   // lo never moves
    ok &= test("above range", {1, 3}, 4, -1);                   // lower bound lands on n
    ok &= test("first and last", {1, 3, 5, 7}, 7, 3);           // last index reachable
    ok &= test("duplicates", {1, 2, 2, 2, 2, 3}, 2, 1);         // leftmost of a run, not mid
    ok &= test("all equal", {2, 2, 2, 2, 2}, 2, 0);             // run covers whole array
    ok &= test("int max", {INT_MIN, 0, INT_MAX}, INT_MAX, 2);   // extreme values
    ok &= test("int min", {INT_MIN, 0, INT_MAX}, INT_MIN, 0);   // extreme values
    cout << (ok ? "all tests passed\n" : "some tests FAILED\n");
    return ok ? 0 : 1;
}
