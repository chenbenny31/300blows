// LC 295: addNum(num) adds an integer from a stream; findMedian() returns the median so
// far (mean of the two middle values when the count is even).
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <cmath>
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
        return (low.top() + high.top()) / 2.0;
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    MedianFinder mf;
    mf.addNum(1);
    check(fabs(mf.findMedian() - 1.0) < 1e-9, "one value");
    mf.addNum(2);
    check(fabs(mf.findMedian() - 1.5) < 1e-9, "even count");
    mf.addNum(3);
    check(fabs(mf.findMedian() - 2.0) < 1e-9, "odd count");
    MedianFinder desc;
    desc.addNum(3);
    desc.addNum(2);
    desc.addNum(1);
    check(fabs(desc.findMedian() - 2.0) < 1e-9, "descending");
}
