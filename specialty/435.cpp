// LC 435 Non-overlapping Intervals: min number of intervals to remove so the rest do not
// overlap; touching ones like [1,2] and [2,3] do not overlap.
// Trick: sort by END and greedily keep the earliest-ending one; remove what overlaps it
// Recognize: max set of non-overlapping intervals (activity selection)
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::sort;

// The earliest end leaves the most room for the rest. T: O(n log n), S: O(1) extra.
int eraseOverlapIntervals(vector<vector<int>> intervals) {
    sort(intervals.begin(), intervals.end(),
         [](const vector<int>& a, const vector<int>& b) { return a[1] < b[1]; });
    int removed = 0;
    int end = intervals[0][1];   // end of the last kept interval
    for (int i = 1; i < (int)intervals.size(); i++) {
        // start == end only touches: keep it
        if (intervals[i][0] < end) {
            removed++;
        } else {
            end = intervals[i][1];
        }
    }
    return removed;
}

void test(const string& label, const vector<vector<int>>& intervals, int want) {
    cout << label << (eraseOverlapIntervals(intervals) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {{1, 2}, {2, 3}, {3, 4}, {1, 3}}, 1);
    test("duplicates", {{1, 2}, {1, 2}, {1, 2}}, 2);
    test("touching", {{1, 2}, {2, 3}}, 0);
}
