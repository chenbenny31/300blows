// min rooms so overlapping meetings never share one, a room frees at end and can be reused at start == end
// follow-up: buses with depart/arrive stations, a bus is reusable only where it parked

// end == start reuses the room?
// closed or half-open intervals? zero-length?
// input sorted? empty?
// empty repositioning between stations allowed? turnaround time? every trip duration > 0?
// return count only, or the assignment?

#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <cassert>
#include <iostream>

using std::vector;

class IntervalScheduler {
public:
    // sort by start + min-heap of end times, T: O(NlogN), S: O(N), sorts its own copy
    int minMeetingRooms_1(vector<vector<int>> intervals) {
        std::sort(intervals.begin(), intervals.end());
        std::priority_queue<int, vector<int>, std::greater<int>> ends; // minHeap of end times
        for (const auto& intv : intervals) {
            int curStart = intv[0];
            int curEnd = intv[1];
            if (!ends.empty() && ends.top() <= curStart) { // <= : room freed at start time is reusable
                ends.pop();
            }
            ends.push(curEnd);
        }
        return static_cast<int>(ends.size()); // never shrinks, so final size is the peak
    }

    // two sorted arrays, T: O(NlogN), S: O(N)
    int minMeetingRooms_2(const vector<vector<int>>& intervals) {
        vector<int> starts;
        vector<int> ends;
        for (const auto& intv : intervals) {
            starts.push_back(intv[0]);
            ends.push_back(intv[1]);
        }
        std::sort(starts.begin(), starts.end());
        std::sort(ends.begin(), ends.end());

        int rooms = 0;
        int i = 0; // earliest end not yet reused
        for (int start : starts) {
            if (start >= ends[i]) {
                i++; // reuse the room that ended
            } else {
                rooms++;
            }
        }
        return rooms;
    }

    // sweep-line over events, T: O(NlogN), S: O(N)
    int minMeetingRooms_3(const vector<vector<int>>& intervals) {
        vector<std::pair<int, int>> events; // event: {start, +1} or {end, -1}
        for (const auto& intv : intervals) {
            events.push_back({intv[0], +1});
            events.push_back({intv[1], -1});
        }
        std::sort(events.begin(), events.end()); // same time: -1 before +1, free room before reuse

        int curRooms = 0;
        int maxRooms = 0;
        for (const auto& e : events) {
            curRooms += e.second;
            maxRooms = std::max(maxRooms, curRooms);
        }
        return maxRooms;
    }

    // follow-up: trip = {depStation, arrStation, depTime, arrTime}, needs arrTime > depTime
    // per-station min-heap of free times, T: O(NlogN), S: O(N), sorts its own copy
    int minBuses_1(vector<vector<int>> trips) {
        std::sort(trips.begin(), trips.end(), [](const vector<int>& a, const vector<int>& b) {
            return a[2] < b[2]; // by departure time
        });
        std::unordered_map<int, std::priority_queue<int, vector<int>, std::greater<int>>> parked; // station -> times buses become free there
        int buses = 0;
        for (const auto& t : trips) {
            auto& here = parked[t[0]]; // [] inserts an empty heap, harmless here
            if (!here.empty() && here.top() <= t[2]) {
                here.pop(); // reuse a bus already at this station
            } else {
                buses++;
            }
            parked[t[1]].push(t[3]); // bus parks at the arrival station
        }
        return buses;
    }

    // sweep per station: buses = sum over stations of peak (departures - arrivals), T: O(NlogN), S: O(N)
    int minBuses_2(vector<vector<int>> trips) {
        std::unordered_map<int, vector<std::pair<int, int>>> events; // station -> event: {depart, +1} or {arrive, -1}
        for (const auto& t : trips) {
            events[t[0]].push_back({t[2], +1});
            events[t[1]].push_back({t[3], -1});
        }

        int buses = 0;
        for (auto& e : events) {
            auto& pairs = e.second;
            std::sort(pairs.begin(), pairs.end());
            int need = 0;
            int peak = 0; // buses must be parted here at start
            for (const auto& p : pairs) {
                need += p.second;
                peak = std::max(peak, need);
            }
            buses += peak;
        }
        return buses;
    }
};

int main() {
    IntervalScheduler is;

    assert(is.minMeetingRooms_1({{0, 30}, {5, 10}, {15, 20}}) == 2); // LC example
    assert(is.minMeetingRooms_2({{0, 30}, {5, 10}, {15, 20}}) == 2);
    assert(is.minMeetingRooms_3({{0, 30}, {5, 10}, {15, 20}}) == 2);
    assert(is.minMeetingRooms_1({{1, 5}, {5, 10}}) == 1); // end == start reuses
    assert(is.minMeetingRooms_2({{1, 5}, {5, 10}}) == 1);
    assert(is.minMeetingRooms_3({{1, 5}, {5, 10}}) == 1);
    assert(is.minMeetingRooms_1({{1, 10}, {2, 9}, {3, 8}}) == 3); // nested, all overlap
    assert(is.minMeetingRooms_2({{1, 10}, {2, 9}, {3, 8}}) == 3);
    assert(is.minMeetingRooms_3({{1, 10}, {2, 9}, {3, 8}}) == 3);
    assert(is.minMeetingRooms_1({}) == 0); // empty
    assert(is.minMeetingRooms_2({}) == 0);
    assert(is.minMeetingRooms_3({}) == 0);

    assert(is.minBuses_1({{0, 1, 0, 10}, {1, 0, 10, 20}}) == 1); // returns from station 1, arrive == depart reuses
    assert(is.minBuses_2({{0, 1, 0, 10}, {1, 0, 10, 20}}) == 1);
    assert(is.minBuses_1({{0, 1, 0, 10}, {0, 1, 10, 20}}) == 2); // bus parked at 1, can't serve station 0
    assert(is.minBuses_2({{0, 1, 0, 10}, {0, 1, 10, 20}}) == 2);
    assert(is.minBuses_1({{0, 1, 0, 10}, {1, 2, 5, 15}}) == 2); // not at station 1 yet at time 5
    assert(is.minBuses_2({{0, 1, 0, 10}, {1, 2, 5, 15}}) == 2);
    assert(is.minBuses_1({}) == 0); // empty
    assert(is.minBuses_2({}) == 0);

    std::cout << "all tests passed\n";
    return 0;
}
