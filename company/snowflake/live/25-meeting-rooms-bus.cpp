// I01 (LC 252 / 253): half-open [start, end) meetings; can one room hold all, and the min rooms needed.
// Follow-up: trips {startTime, endTime, fromStation, toStation}; min buses, a bus is reused only where it parked.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <functional>
#include <climits>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::priority_queue;
using std::pair;

class MeetingScheduler {
public:
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
        priority_queue<int, vector<int>, std::greater<int>> ends;   // end time of each room's current meeting
        for (const auto& m : intervals) {
            if (!ends.empty() && ends.top() <= m[0]) { ends.pop(); }   // <=: room freed at start is reusable
            ends.push(m[1]);
        }
        return ends.size();   // the heap never shrinks, so its size is the peak
    }

    // sweep over +1 / -1 events, T: O(NlogN), S: O(N)
    int minRooms2(const vector<vector<int>>& intervals) {
        vector<pair<int, int>> events;   // (time, +1 start or -1 end)
        for (const auto& m : intervals) {
            events.push_back({m[0], 1});
            events.push_back({m[1], -1});
        }
        std::sort(events.begin(), events.end());   // same time: -1 sorts before +1, free before reuse
        int cur = 0;
        int peak = 0;
        for (const auto& e : events) {
            cur += e.second;
            peak = std::max(peak, cur);
        }
        return peak;
    }

    // sort by start + per-station min-heap of idle-from times, T: O(NlogN), S: O(N)
    int minBuses(vector<vector<int>> trips) {
        std::sort(trips.begin(), trips.end());   // trip[0] = startTime first
        unordered_map<int, priority_queue<int, vector<int>, std::greater<int>>> parked;   // station -> times buses become idle there
        int buses = 0;
        for (const auto& t : trips) {
            int start = t[0], end = t[1], from = t[2], to = t[3];
            auto& here = parked[from];
            if (!here.empty() && here.top() <= start) {
                here.pop();   // reuse a bus already idle at this station
            } else {
                buses++;
            }
            parked[to].push(end);
        }
        return buses;
    }
};

bool test(const string& label, const vector<vector<int>>& intervals, bool canAttend, int rooms) {
    MeetingScheduler ms;
    bool gotCan = ms.canAttendAll(intervals);
    int got = ms.minRooms(intervals);
    int got2 = ms.minRooms2(intervals);
    if (gotCan == canAttend && got == rooms && got2 == rooms) { return true; }
    cout << "FAIL " << label << ": canAttend " << gotCan << ", rooms " << got << " / " << got2
         << ", expected " << canAttend << ", " << rooms << "\n";
    return false;
}

bool testBuses(const string& label, const vector<vector<int>>& trips, int expected) {
    MeetingScheduler ms;
    int got = ms.minBuses(trips);
    if (got == expected) { return true; }
    cout << "FAIL " << label << ": got " << got << ", expected " << expected << "\n";
    return false;
}

int main() {
    bool ok = true;
    ok &= test("LC example", {{0, 30}, {5, 10}, {15, 20}}, false, 2);       // LC 252 / 253 example
    ok &= test("unsorted disjoint", {{7, 10}, {2, 4}}, true, 1);            // LC 252 example 2, needs sort
    ok &= test("empty", {}, true, 0);                                       // no meetings
    ok &= test("single", {{1, 2}}, true, 1);                                // smallest non-empty
    ok &= test("touching", {{1, 5}, {5, 10}}, true, 1);                     // end == start reuses
    ok &= test("nested", {{1, 10}, {2, 9}, {3, 8}}, false, 3);              // all overlap
    ok &= test("same start", {{1, 2}, {1, 2}, {1, 2}}, false, 3);           // all-equal
    ok &= test("end and two starts at 3", {{1, 3}, {3, 5}, {3, 4}}, false, 2); // sweep tie order matters

    ok &= testBuses("round trip", {{0, 10, 0, 1}, {10, 20, 1, 0}}, 1);       // arrive == depart reuses
    ok &= testBuses("wrong station", {{0, 10, 0, 1}, {10, 20, 0, 1}}, 2);    // bus parked at 1, not 0
    ok &= testBuses("not arrived yet", {{0, 10, 0, 1}, {5, 15, 1, 2}}, 2);   // at station 1 only from 10
    ok &= testBuses("empty", {}, 0);                                         // no trips
    ok &= testBuses("unsorted chain", {{20, 30, 2, 0}, {0, 10, 0, 1}, {10, 20, 1, 2}}, 1); // must sort by start
    ok &= testBuses("supply at station 1",
                    {{0, 10, 0, 1}, {1, 11, 0, 1}, {12, 20, 1, 0}, {12, 20, 1, 0}, {13, 14, 1, 0}}, 3); // 2 arrive, 3 leave
    cout << (ok ? "all tests passed\n" : "some tests FAILED\n");
    return ok ? 0 : 1;
}
