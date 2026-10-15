// LC 528 Random Pick with Weight: given positive weights w, pickIndex() returns i with
// probability w[i] / sum(w).
// Trick: prefix sums tile [1, total]; draw r, binary search the first prefix >= r
// Recognize: weighted random choice -> prefix sums + binary search.
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
using std::cout;
using std::string;
using std::vector;
using std::rand;

class Solution {
    vector<int> prefix;   // prefix[i] = w[0] + ... + w[i]

public:
    // T: O(n), S: O(n).
    Solution(const vector<int>& w) {
        int sum = 0;
        for (int x : w) {
            sum += x;
            prefix.push_back(sum);
        }
    }

    // binary search the first i with prefix[i] >= r. T: O(log n).
    int indexFor(int r) {
        int lo = 0, hi = (int)prefix.size() - 1;
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (prefix[mid] >= r) {
                hi = mid;
            } else {
                lo = mid + 1;
            }
        }
        return lo;
    }

    // r in [1, total]: index i owns (prefix[i-1], prefix[i]], w[i] values. T: O(log n).
    int pickIndex() { return indexFor(rand() % prefix.back() + 1); }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    Solution a({1, 3});
    check(a.indexFor(1) == 0 && a.indexFor(2) == 1 && a.indexFor(4) == 1, "two weights");
    Solution b({2, 5, 3});
    check(b.indexFor(2) == 0 && b.indexFor(3) == 1 && b.indexFor(7) == 1, "boundaries");
    check(b.indexFor(8) == 2 && b.indexFor(10) == 2, "last bucket");
    int i = b.pickIndex();
    check(i >= 0 && i < 3, "pick in range");
}
