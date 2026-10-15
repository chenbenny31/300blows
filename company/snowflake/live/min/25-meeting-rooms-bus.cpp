// I01 · LC 252 / 253: half-open [start, end) meetings: can one room hold all, min rooms. Follow-up: trips {start, end, from, to}, min buses.
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <functional>
using std::cout;
using std::vector;
using std::priority_queue;
using std::unordered_map;
using MinHeap = priority_queue<int, vector<int>, std::greater<int>>;

// sort by start, compare neighbors, T: O(NlogN), S: O(N) for the copy
bool canAttendAll(vector<vector<int>> intervals) {
    std::sort(intervals.begin(), intervals.end());
    int n = intervals.size();
    for (int i = 1; i < n; i++) {
        if (intervals[i][0] < intervals[i - 1][1]) { return false; }   // end == start is not a conflict
    }
    return true;
}

// sort by start + min-heap of end times, T: O(NlogN), S: O(N)
int minRooms(vector<vector<int>> intervals) {
    std::sort(intervals.begin(), intervals.end());
    MinHeap ends;   // end time of each room's current meeting
    for (const auto& m : intervals) {
        if (!ends.empty() && ends.top() <= m[0]) { ends.pop(); }   // <=: room freed at start is reusable
        ends.push(m[1]);
    }
    return ends.size();   // the heap never shrinks, so its size is the peak
}

// follow-up: sort by start + per-station min-heap of idle-from times, T: O(NlogN), S: O(N)
int minBuses(vector<vector<int>> trips) {
    std::sort(trips.begin(), trips.end());   // trip[0] = startTime first
    unordered_map<int, MinHeap> parked;      // station -> times buses become idle there
    int buses = 0;
    for (const auto& t : trips) {
        MinHeap& here = parked[t[2]];
        if (!here.empty() && here.top() <= t[0]) {
            here.pop();                      // reuse a bus already idle at this station
        } else {
            buses++;
        }
        parked[t[3]].push(t[1]);
    }
    return buses;
}

void testAttend(const vector<vector<int>>& intervals, bool expect) {
    bool got = canAttendAll(intervals);
    cout << (got == expect ? "PASS " : "FAIL ") << got << " expect " << expect << "\n";
}

void testRooms(const vector<vector<int>>& intervals, int expect) {
    int got = minRooms(intervals);
    cout << (got == expect ? "PASS " : "FAIL ") << got << " expect " << expect << "\n";
}

void testBuses(const vector<vector<int>>& trips, int expect) {
    int got = minBuses(trips);
    cout << (got == expect ? "PASS " : "FAIL ") << got << " expect " << expect << "\n";
}

int main() {
    testAttend({{0, 30}, {5, 10}, {15, 20}}, false);              // LC 252 example
    testAttend({{7, 10}, {2, 4}}, true);                          // unsorted disjoint
    testAttend({{1, 5}, {5, 10}}, true);                          // touching is fine

    testRooms({{0, 30}, {5, 10}, {15, 20}}, 2);                   // LC 253 example
    testRooms({}, 0);                                             // no meetings
    testRooms({{1, 10}, {2, 9}, {3, 8}}, 3);                      // nested: all overlap
    testRooms({{1, 3}, {3, 5}, {3, 4}}, 2);                       // end == start frees the room

    testBuses({{0, 10, 0, 1}, {10, 20, 1, 0}}, 1);                // arrive == depart reuses
    testBuses({{0, 10, 0, 1}, {10, 20, 0, 1}}, 2);                // bus parked at 1, not 0
    testBuses({{0, 10, 0, 1}, {5, 15, 1, 2}}, 2);                 // not arrived yet
    testBuses({{20, 30, 2, 0}, {0, 10, 0, 1}, {10, 20, 1, 2}}, 1);   // must sort by start
}
