// LC 4 Median of Two Sorted Arrays: return the median of the two sorted arrays combined
// in O(log(min(m, n))).
// Trick: binary search how many of the shorter array go left so maxLeft <= minRight
// Recognize: k-th / median of two sorted arrays in log time -> binary search a partition
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>
#include <cmath>
using std::cout;
using std::string;
using std::vector;
using std::max;
using std::min;
using std::fabs;

// Fixing i from a fixes j = half - i from b; a valid cut has both cross pairs ordered.
// T: O(log(min(m, n))), S: O(1).
double findMedianSortedArrays(const vector<int>& a, const vector<int>& b) {
    // search the shorter array so j = half - i stays inside b
    if (a.size() > b.size()) { return findMedianSortedArrays(b, a); }
    int m = a.size(), n = b.size();
    int half = (m + n + 1) / 2;   // size of the combined left half
    int lo = 0, hi = m;           // range for i = elements of a in the left half
    while (lo <= hi) {
        int i = (lo + hi) / 2;
        int j = half - i;         // elements of b in the left half
        // sentinels: an empty side never blocks the cut
        int aLeft = i == 0 ? INT_MIN : a[i - 1];
        int aRight = i == m ? INT_MAX : a[i];
        int bLeft = j == 0 ? INT_MIN : b[j - 1];
        int bRight = j == n ? INT_MAX : b[j];
        if (aLeft > bRight) {
            hi = i - 1;
        } else if (bLeft > aRight) {
            lo = i + 1;
        } else {
            int leftMax = max(aLeft, bLeft);
            if ((m + n) % 2 == 1) { return leftMax; }
            // 0.0 first: int addition could overflow
            return (0.0 + leftMax + min(aRight, bRight)) / 2;
        }
    }
    return 0;   // unreachable for sorted input
}

void test(const string& label, const vector<int>& a, const vector<int>& b, double want) {
    double got = findMedianSortedArrays(a, b);
    cout << label << (fabs(got - want) < 1e-9 ? " PASS\n" : " FAIL\n");
}

int main() {
    test("odd total", {1, 3}, {2}, 2.0);
    test("even total", {1, 2}, {3, 4}, 2.5);
    test("first empty", {}, {1}, 1.0);
    test("second empty", {2}, {}, 2.0);
}
