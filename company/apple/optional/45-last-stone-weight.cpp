// LC 1046: repeatedly smash the two heaviest stones x <= y; y - x survives if nonzero.
// Return the last stone's weight, or 0 if none is left.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
using std::cout;
using std::string;
using std::vector;
using std::priority_queue;

// Max-heap: pop the two heaviest, push back the difference. T: O(n log n), S: O(n).
int lastStoneWeight(const vector<int>& stones) {
    priority_queue<int> heap(stones.begin(), stones.end());   // top = heaviest stone
    while (heap.size() > 1) {
        int y = heap.top();
        heap.pop();
        int x = heap.top();
        heap.pop();
        if (y > x) { heap.push(y - x); }
    }
    return heap.empty() ? 0 : heap.top();
}

void test(const string& label, const vector<int>& stones, int want) {
    cout << label << (lastStoneWeight(stones) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {2, 7, 4, 1, 8, 1}, 1);
    test("all gone", {2, 2}, 0);
    test("one stone", {1}, 1);
}
