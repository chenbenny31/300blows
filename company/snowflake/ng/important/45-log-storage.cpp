// E01 (IC1/IC2) · LC 635: put(id, timestamp) with "Year:Month:Day:Hour:Minute:Second"
// (zero-padded); retrieve(start, end, granularity) = ids whose timestamp lies in
// [start, end] when both are compared only down to granularity (Year .. Second).
// Follow-up: faster retrieve -> logs kept sorted by timestamp, one range scan.
#include <iostream>
#include <vector>
#include <string>
#include <map>
using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::multimap;

// characters of "YYYY:MM:DD:HH:MM:SS" that matter at each granularity
int prefixLen(const string& granularity) {
    if (granularity == "Year") { return 4; }
    if (granularity == "Month") { return 7; }
    if (granularity == "Day") { return 10; }
    if (granularity == "Hour") { return 13; }
    if (granularity == "Minute") { return 16; }
    return 19;
}

// Scan every log and compare truncated strings. T: put O(1), retrieve O(N), S: O(N).
class LogSystem {
    vector<pair<string, int>> logs;   // (timestamp, id) in insertion order

public:
    void put(int id, const string& timestamp) { logs.push_back({timestamp, id}); }

    vector<int> retrieve(const string& start, const string& end, const string& gran) {
        int len = prefixLen(gran);
        string lo = start.substr(0, len);
        string hi = end.substr(0, len);
        vector<int> ids;
        for (const auto& [timestamp, id] : logs) {
            // fixed-width and zero-padded, so string order is time order
            string t = timestamp.substr(0, len);
            if (lo <= t && t <= hi) { ids.push_back(id); }
        }
        return ids;
    }
};

// follow-up: sorted by timestamp; pad the bounds, then one range scan.
// T: put O(log N), retrieve O(log N + K), S: O(N).
class LogSystemSorted {
    // timestamp -> id, sorted by time; multi: two logs may share a timestamp
    multimap<string, int> logs;

public:
    void put(int id, const string& timestamp) { logs.insert({timestamp, id}); }

    vector<int> retrieve(const string& start, const string& end, const string& gran) {
        int len = prefixLen(gran);
        string lowPad = "0000:00:00:00:00:00";
        string highPad = "9999:99:99:99:99:99";
        // lo sits below every real suffix, hi above, so the end unit stays inclusive
        string lo = start.substr(0, len) + lowPad.substr(len);
        string hi = end.substr(0, len) + highPad.substr(len);
        vector<int> ids;
        for (auto it = logs.lower_bound(lo); it != logs.end() && it->first <= hi; ++it) {
            ids.push_back(it->second);
        }
        return ids;
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    LogSystem s;
    string start = "2016:01:01:01:01:01";
    string end = "2017:01:01:23:00:00";
    check(s.retrieve(start, end, "Year").empty(), "no logs");
    s.put(1, "2017:01:01:23:59:59");
    s.put(2, "2017:01:01:22:59:59");
    s.put(3, "2016:01:01:00:00:00");
    check(s.retrieve(start, end, "Year") == vector<int>{1, 2, 3}, "lc year");
    check(s.retrieve(start, end, "Hour") == vector<int>{1, 2}, "lc hour");
    check(s.retrieve("2018:01:01:00:00:00", end, "Year").empty(), "start after end");
}
