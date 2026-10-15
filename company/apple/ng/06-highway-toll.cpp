// Apple (hack2hire): logs (plate, checkpoint, time), unsorted. A trip between consecutive
// checkpoints (by time) costs 2.0 + 0.1 * euclidean distance. Return plate -> total toll.
// Follow-up: a gap > T between consecutive logs means the car left; that segment is free.
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <cmath>
using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::map;
using std::unordered_map;
using std::sort;
using std::sqrt;
using std::fabs;

struct Log {
    string plate;
    string checkpoint;
    int time;
};

// given helper in the problem; stubbed with the example's coordinates
pair<double, double> getCoordinates(const string& checkpoint) {
    unordered_map<string, pair<double, double>> coords = {
        {"A", {0, 0}}, {"B", {3, 4}}, {"C", {6, 8}}};
    return coords[checkpoint];
}

double segmentToll(const string& from, const string& to) {
    auto [x1, y1] = getCoordinates(from);
    auto [x2, y2] = getCoordinates(to);
    return 2.0 + 0.1 * sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

// group by plate, sort each plate's passes by time, charge every consecutive pair.
// T: O(n log n), S: O(n).
map<string, double> tolls(const vector<Log>& logs) {
    // plate -> (time, checkpoint) passes
    unordered_map<string, vector<pair<int, string>>> passes;
    for (const auto& log : logs) {
        passes[log.plate].push_back({log.time, log.checkpoint});
    }
    map<string, double> result;   // plate -> total toll
    for (auto& [plate, p] : passes) {
        // input is not time-ordered
        sort(p.begin(), p.end());
        result[plate] = 0;
        for (int i = 1; i < (int)p.size(); i++) {
            result[plate] += segmentToll(p[i - 1].second, p[i].second);
        }
    }
    return result;
}

// follow-up: a gap > T splits trips, so that pair is not charged. T: O(n log n).
map<string, double> tollsWithGap(const vector<Log>& logs, int T) {
    // plate -> (time, checkpoint) passes
    unordered_map<string, vector<pair<int, string>>> passes;
    for (const auto& log : logs) {
        passes[log.plate].push_back({log.time, log.checkpoint});
    }
    map<string, double> result;
    for (auto& [plate, p] : passes) {
        sort(p.begin(), p.end());
        result[plate] = 0;
        for (int i = 1; i < (int)p.size(); i++) {
            if (p[i].first - p[i - 1].first > T) { continue; }
            result[plate] += segmentToll(p[i - 1].second, p[i].second);
        }
    }
    return result;
}

void test(const string& label, const vector<Log>& logs, const map<string, double>& want) {
    map<string, double> got = tolls(logs);
    bool ok = got.size() == want.size();
    for (const auto& [plate, toll] : want) {
        ok = ok && got.count(plate) && fabs(got[plate] - toll) < 1e-9;
    }
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("example", {{"C1", "A", 1}, {"C1", "B", 3}, {"C1", "C", 5}, {"C2", "A", 2},
                     {"C2", "B", 6}}, {{"C1", 5.0}, {"C2", 2.5}});
    test("unsorted", {{"C1", "C", 5}, {"C1", "A", 1}, {"C1", "B", 3}}, {{"C1", 5.0}});
    test("one pass", {{"C1", "A", 1}}, {{"C1", 0.0}});
}
