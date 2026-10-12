#include <algorithm>
#include <cassert>
#include <unordered_map>
#include <utility>
#include <vector>

using std::pair;
using std::unordered_map;
using std::vector;

// T: O(nlogn), S: O(n)
int minBuses(const vector<vector<int>>& trips) {
    unordered_map<int, vector<pair<int, int>>> events; // station -> [(time, change),]

    for (const auto& t : trips) {
        events[t[0]].push_back({t[2], +1}); // departure
        events[t[1]].push_back({t[3], -1}); // arrival
    }

    int total = 0;
    for (auto& [station, timeline] : events) {
        std::sort(timeline.begin(), timeline.end()); // arrival (-1) precedes departure (+1)

        int delta = 0;
        int need = 0;
        for (auto& e : timeline) {
            delta += e.second;
            need = std::max(need, delta);
        }

        total += need;
    }
    return total;
}

int main() {
    // Same bus arrives at station 1 exactly when its next trip departs.
    assert(minBuses({{0, 1, 0, 10}, {1, 0, 10, 20}}) == 1);

    // Bus is at station 1, so station 0 needs another.
    assert(minBuses({{0, 1, 0, 10}, {0, 1, 10, 20}}) == 2);

    // Arrival at station 1 is too late for its departure.
    assert(minBuses({{0, 1, 0, 10}, {1, 2, 5, 15}}) == 2);

    assert(minBuses({}) == 0);
}
