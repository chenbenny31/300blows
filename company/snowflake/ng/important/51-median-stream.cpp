// LC 295: addNum(num) adds an integer from a stream; findMedian() returns the median so
// far (mean of the two middle values when the count is even).
// Follow-up (LC): every value is in [0, 100] -> counting buckets instead of heaps.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <cmath>
#include <climits>
using std::cout;
using std::string;
using std::vector;
using std::priority_queue;
using std::greater;
using std::fabs;

class MedianFinder {
    priority_queue<int> low;                               // max-heap: smaller half
    priority_queue<int, vector<int>, greater<int>> high;   // min-heap: larger half

public:
    // Pass through low into high, then rebalance so low keeps the extra one.
    // T: O(log n).
    void addNum(int num) {
        // routing via low keeps every value in low <= every value in high
        low.push(num);
        high.push(low.top());
        low.pop();
        if (high.size() > low.size()) {
            low.push(high.top());
            high.pop();
        }
    }

    // T: O(1).
    double findMedian() {
        if (low.size() > high.size()) { return low.top(); }
        return (1LL * low.top() + high.top()) / 2.0;   // widen: INT_MAX + INT_MAX
    }
};

// follow-up: count each value, walk the buckets to the middle rank.
// T: addNum O(1), findMedian O(101); S: O(101).
class MedianFinderBuckets {
    vector<int> cnt;   // cnt[v] = how many times v was added
    int total = 0;     // numbers added so far

    // value at 0-based rank k in sorted order
    int kth(int k) {
        for (int v = 0; v <= 100; v++) {
            k -= cnt[v];
            if (k < 0) { return v; }
        }
        return -1;
    }

public:
    MedianFinderBuckets() : cnt(101, 0) {}

    void addNum(int num) {
        cnt[num]++;
        total++;
    }

    // odd total: both ranks are the same middle one
    double findMedian() { return (kth((total - 1) / 2) + kth(total / 2)) / 2.0; }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    MedianFinder mf;
    mf.addNum(1);
    mf.addNum(2);
    check(fabs(mf.findMedian() - 1.5) < 1e-9, "even count");
    mf.addNum(3);
    check(fabs(mf.findMedian() - 2.0) < 1e-9, "odd count");
    MedianFinder desc;
    desc.addNum(3);
    desc.addNum(2);
    desc.addNum(1);
    check(fabs(desc.findMedian() - 2.0) < 1e-9, "descending");
    MedianFinder big;
    big.addNum(INT_MAX);
    big.addNum(INT_MAX);
    check(fabs(big.findMedian() - INT_MAX) < 1e-9, "no overflow");
}
