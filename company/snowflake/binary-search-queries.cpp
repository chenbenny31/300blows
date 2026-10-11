#include <cassert>
#include <vector>

using std::vector;

// T: O(logn), S: O(1)
int search(const vector<int>& a, int target) {
    int n = static_cast<int>(a.size());
    int lo = 0;
    int hi = n; // [0, n)

    while (lo < hi) { // find the first elem >= target
        int mid = lo + (hi - lo) / 2;
        if (a[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid; // mid stays in case a[mid]==target
        }
    }
    return lo < n && a[lo] == target ? lo : -1; // lowerBound need ==target validation
}

int main() {
    vector<int> a = {1, 3, 5, 5, 5, 9};

    assert(search(a, 5) == 2);
    // missing target: -1
    // first elem: 0
    // last elem: 5
    // below min: -1
    // above max: -1
    // empty: -1
    // single elem search hit: 0
}
