// numbers arrive one by one: addNum(num), findMedian() returns the median so far (LC295)
// even count: average of the two middle values

// findMedian never called on an empty stream?
// int range, two INT_MAX values averaged?
// return double?
// deletions or a sliding window later?

#include <vector>
#include <queue>
#include <functional>
#include <climits>
#include <cassert>
#include <iostream>

using std::vector;

// T: addNum O(logn), findMedian O(1), S: O(n)

class MedianFinder {
private:
    std::priority_queue<int> low; // max-heap, smaller half, holds the extra one on odd counts
    std::priority_queue<int, vector<int>, std::greater<int>> high; // min-heap, larger half

public:
    // invariants: every low <= every high, low.size() == high.size() or high.size() + 1
    void addNum(int num) {
        low.push(num);
        high.push(low.top()); // largest of the low half moves up, keeps the order invariant
        low.pop();
        if (high.size() > low.size()) {
            low.push(high.top()); // rebalance so low keeps the extra
            high.pop();
        }
    }

    double findMedian() const {
        if (low.size() > high.size()) {
            return low.top();
        }
        return (static_cast<long long>(low.top()) + high.top()) / 2.0; // widen first, INT_MAX + INT_MAX overflows
    }
};

int main() {
    MedianFinder mf;
    mf.addNum(1);
    mf.addNum(2);
    assert(mf.findMedian() == 1.5); // target: even count averages the middle two
    mf.addNum(3);
    assert(mf.findMedian() == 2); // target: odd count

    MedianFinder dup;
    dup.addNum(-1);
    dup.addNum(-1);
    assert(dup.findMedian() == -1); // edge: duplicates, negatives

    MedianFinder big;
    big.addNum(INT_MAX);
    big.addNum(INT_MAX);
    assert(big.findMedian() == INT_MAX); // fault: sum overflows int

    MedianFinder one;
    one.addNum(5);
    assert(one.findMedian() == 5); // null: single number

    std::cout << "all tests passed\n";
    return 0;
}
