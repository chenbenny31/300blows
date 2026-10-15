// LC 56: merge all overlapping closed intervals [start, end];
// touching ones like [1,2] and [2,3] merge too. Input is not sorted.
// Follow-up LC 57: insert newInterval into sorted, disjoint intervals and merge.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::sort;
using std::max;
using std::min;

// Sort by start, then extend the last merged interval. T: O(n log n), S: O(n).
vector<vector<int>> merge(vector<vector<int>> intervals) {
    sort(intervals.begin(), intervals.end());
    vector<vector<int>> merged;   // disjoint intervals in start order
    for (const auto& cur : intervals) {
        // closed intervals: start == last end still overlaps
        if (!merged.empty() && cur[0] <= merged.back()[1]) {
            // max: cur may sit entirely inside the last one
            merged.back()[1] = max(merged.back()[1], cur[1]);
        } else {
            merged.push_back(cur);
        }
    }
    return merged;
}

// follow-up: copy those ending before, absorb overlaps, copy the rest. T: O(n), S: O(n).
vector<vector<int>> insert(const vector<vector<int>>& intervals,
                           vector<int> newInterval) {
    vector<vector<int>> result;   // disjoint intervals in start order
    int n = intervals.size();
    int i = 0;
    while (i < n && intervals[i][1] < newInterval[0]) {
        result.push_back(intervals[i++]);
    }
    // overlaps: grow newInterval to cover each one
    while (i < n && intervals[i][0] <= newInterval[1]) {
        newInterval[0] = min(newInterval[0], intervals[i][0]);
        newInterval[1] = max(newInterval[1], intervals[i][1]);
        i++;
    }
    result.push_back(newInterval);
    while (i < n) {
        result.push_back(intervals[i++]);
    }
    return result;
}

void test(const string& label, const vector<vector<int>>& intervals,
          const vector<vector<int>>& want) {
    cout << label << (merge(intervals) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("unsorted", {{8, 10}, {1, 3}, {2, 6}}, {{1, 6}, {8, 10}});
    test("touching", {{1, 2}, {2, 3}}, {{1, 3}});
    test("contained", {{1, 10}, {2, 3}}, {{1, 10}});
    test("empty", {}, {});
}
