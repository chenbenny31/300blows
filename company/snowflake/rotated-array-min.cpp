#include <cassert>
#include <vector>

using std::vector;

// T: O(logn), S: O(1)
int findMin(const vector<int>& nums) {
    int lo = 0;
    int hi = static_cast<int>(nums.size()) - 1;

    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (nums[mid] > nums[hi]) { // min must be after mid
            lo = mid + 1;
        } else { // min at mid or before it
            hi = mid;
        }
    }
    return nums[lo];
}

// T: O(n), S: O(1)
int findMin_dup(const vector<int>& nums) {
    int lo = 0;
    int hi = static_cast<int>(nums.size()) - 1;

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

int main() {
    assert(findMin({4, 5, 6, 7, 0, 1, 2}) == 0); // Rotated.
    assert(findMin({1, 2, 3}) == 1); // Already sorted.
    assert(findMin({2, 1}) == 1); // Minimum at last position.
    assert(findMin({5}) == 5); // Single element.

    assert(findMin_dup({10, 1, 10, 10, 10}) == 1);
    assert(findMin_dup({2, 2, 2}) == 2);
}
