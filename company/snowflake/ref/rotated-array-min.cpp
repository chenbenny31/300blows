// sorted ascending array rotated at an unknown pivot, find the minimum (LC153, J13422)
// follow-up LC154: duplicates allowed

// distinct values? (153 yes, 154 no)
// array non-empty?
// maybe not rotated at all?
// return the value or its index?

#include <vector>
#include <algorithm>
#include <cassert>
#include <iostream>

using std::vector;

class RotatedSearch {
public:
    // 1: baseline scan, T: O(n), S: O(1)
    int findMin_1(const vector<int>& a) {
        return *std::min_element(a.begin(), a.end());
    }

    // 2: [lo, hi] always holds the min; a[mid] > a[hi] means the drop is right of mid
    // T: O(logn), S: O(1)
    int findMin_2(const vector<int>& a) {
        int lo = 0;
        int hi = static_cast<int>(a.size()) - 1;
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (a[mid] > a[hi]) {
                lo = mid + 1; // mid is in the left, larger part
            } else {
                hi = mid; // mid is in the right part, could be the min itself
            }
        }
        return a[lo];
    }

    // 3 (LC154): duplicates, a[mid] == a[hi] says nothing about the side, drop hi only
    // T: O(logn) typical, O(n) worst (all equal), S: O(1)
    int findMinDup(const vector<int>& a) {
        int lo = 0;
        int hi = static_cast<int>(a.size()) - 1;
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (a[mid] > a[hi]) {
                lo = mid + 1;
            } else if (a[mid] < a[hi]) {
                hi = mid;
            } else {
                hi--; // a[hi] has a copy at mid, safe to drop
            }
        }
        return a[lo];
    }
};

int main() {
    RotatedSearch rs;

    assert(rs.findMin_1({4, 5, 6, 7, 0, 1, 2}) == 0); // target
    assert(rs.findMin_2({4, 5, 6, 7, 0, 1, 2}) == 0);
    assert(rs.findMin_2({1, 2, 3}) == 1); // edge: not rotated
    assert(rs.findMin_2({2, 1}) == 1); // edge: two elements, min last
    assert(rs.findMin_2({5}) == 5); // null: single element

    assert(rs.findMinDup({10, 1, 10, 10, 10}) == 1); // follow-up: a[mid] == a[hi] but the min is left of mid
    assert(rs.findMinDup({2, 2, 2}) == 2); // all equal, worst case

    std::cout << "all tests passed\n";
    return 0;
}
