// put(id, timestamp) with timestamp "YYYY:MM:DD:HH:MM:SS"
// retrieve(start, end, granularity): ids whose timestamp is in [start, end], compared only up to granularity (LC635)
// granularity: Year, Month, Day, Hour, Minute, Second

// timestamps always fixed-width and zero-padded? (string compare == time compare)
// range inclusive on both ends?
// output order?
// same id or same timestamp more than once?
// start after end: empty?

#include <vector>
#include <string>
#include <map>
#include <utility>
#include <algorithm>
#include <cassert>
#include <iostream>

using std::vector;
using std::string;

// characters of "YYYY:MM:DD:HH:MM:SS" that matter at each granularity
int prefixLen(const string& granularity) {
    if (granularity == "Year") {
        return 4;
    }
    if (granularity == "Month") {
        return 7;
    }
    if (granularity == "Day") {
        return 10;
    }
    if (granularity == "Hour") {
        return 13;
    }
    if (granularity == "Minute") {
        return 16;
    }
    return 19;
}

// 1: scan every log, compare truncated strings, T: put O(1), retrieve O(N*L), S: O(N)
class LogSystem_1 {
private:
    vector<std::pair<string, int>> logs; // (timestamp, id)

public:
    void put(int id, const string& timestamp) {
        logs.push_back({timestamp, id});
    }

    vector<int> retrieve(const string& start, const string& end, const string& granularity) {
        int len = prefixLen(granularity);
        string lo = start.substr(0, len);
        string hi = end.substr(0, len);
        vector<int> ids;
        for (const auto& log : logs) {
            string t = log.first.substr(0, len);
            if (lo <= t && t <= hi) { // zero-padded, so string order is time order
                ids.push_back(log.second);
            }
        }
        return ids;
    }
};

// 2: logs ordered by timestamp, pad the bounds and range-scan, T: put O(logN), retrieve O(logN + K), S: O(N)
class LogSystem_2 {
private:
    std::multimap<string, int> logs; // timestamp -> id, sorted by time

public:
    void put(int id, const string& timestamp) {
        logs.insert({timestamp, id});
    }

    vector<int> retrieve(const string& start, const string& end, const string& granularity) {
        int len = prefixLen(granularity);
        string lo = start.substr(0, len) + string("0000:00:00:00:00:00").substr(len); // below any real suffix
        string hi = end.substr(0, len) + string("9999:99:99:99:99:99").substr(len); // above any real suffix
        vector<int> ids;
        for (auto it = logs.lower_bound(lo); it != logs.end() && it->first <= hi; ++it) {
            ids.push_back(it->second);
        }
        return ids;
    }
};

int main() {
    vector<int> want;
    vector<int> got;

    LogSystem_1 a;
    assert(a.retrieve("2016:01:01:01:01:01", "2017:01:01:23:00:00", "Year").empty()); // null: no logs
    a.put(1, "2017:01:01:23:59:59");
    a.put(2, "2017:01:01:22:59:59");
    a.put(3, "2016:01:01:00:00:00");
    want = {1, 2, 3};
    got = a.retrieve("2016:01:01:01:01:01", "2017:01:01:23:00:00", "Year");
    std::sort(got.begin(), got.end());
    assert(got == want); // target: only the year is compared
    want = {1, 2};
    got = a.retrieve("2016:01:01:01:01:01", "2017:01:01:23:00:00", "Hour");
    std::sort(got.begin(), got.end());
    assert(got == want); // edge: 23:59:59 still inside end hour 23, 3 is before start hour 01
    assert(a.retrieve("2018:01:01:00:00:00", "2017:01:01:00:00:00", "Year").empty()); // fault: start after end

    LogSystem_2 b;
    assert(b.retrieve("2016:01:01:01:01:01", "2017:01:01:23:00:00", "Year").empty()); // null
    b.put(1, "2017:01:01:23:59:59");
    b.put(2, "2017:01:01:22:59:59");
    b.put(3, "2016:01:01:00:00:00");
    want = {1, 2, 3};
    got = b.retrieve("2016:01:01:01:01:01", "2017:01:01:23:00:00", "Year");
    std::sort(got.begin(), got.end());
    assert(got == want); // target
    want = {1, 2};
    got = b.retrieve("2016:01:01:01:01:01", "2017:01:01:23:00:00", "Hour");
    std::sort(got.begin(), got.end());
    assert(got == want); // edge
    assert(b.retrieve("2018:01:01:00:00:00", "2017:01:01:00:00:00", "Year").empty()); // fault

    std::cout << "all tests passed\n";
    return 0;
}
