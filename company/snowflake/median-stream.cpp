#include <cassert>
#include <climits>
#include <functional>
#include <queue>
#include <vector>

using std::greater;
using std::priority_queue;
using std::vector;

class MedianFinder {
private:
    priority_queue<int> lower; // smaller half, maxHeap
    priority_queue<int, vector<int>, greater<int>> higher; // larger half, minHeap

public:
    // T: O(logn), S: O(1)
    void addNum(int num) {
        lower.push(num);
        higher.push(lower.top());
        lower.pop();

        if (higher.size() > lower.size()) {
            lower.push(higher.top());
            higher.pop();
        }
    }

    // T: O(1), S: O(1)
    double findMedian() const {
        if (lower.size() > higher.size()) {
            return lower.top();
        }
        return (static_cast<long long>(lower.top()) + higher.top()) / 2.0;
    }
};

int main() {
    MedianFinder median;
    median.addNum(3);
    assert(median.findMedian() == 3); // Single element.

    median.addNum(1);
    assert(median.findMedian() == 2); // Even count, unsorted insertion.

    median.addNum(-1);
    assert(median.findMedian() == 1); // Odd count.

    median.addNum(-1);
    assert(median.findMedian() == 0); // Duplicate and negative values.

    MedianFinder large;
    large.addNum(INT_MAX);
    large.addNum(INT_MAX);
    assert(large.findMedian() == INT_MAX); // Avoid overflow when averaging.
}
