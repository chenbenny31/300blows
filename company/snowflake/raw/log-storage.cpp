#include <cassert>
#include <string>
#include <utility>
#include <vector>

using std::pair;
using std::string;
using std::vector;

int prefixLength(const string& granularity) {
    if (granularity == "Year") { return 4;}
    if (granularity == "Month") { return 7;}
    if (granularity == "Day") { return 10;}
    if (granularity == "Hour") { return 13;}
    if (granularity == "Minute") { return 16;}
    return 19;
}

class LogSystem {
private:
    vector<pair<string, int>> logs; // [(ts, id),]

public:
    // T: O(1), S: O(1)
    void put(int id, const string& timestamp) {
        logs.push_back({timestamp, id});
    }

    vector<int> retrieve(const string& start, const string& end,
                         const string& granularity) const {
        int length = prefixLength(granularity);
        string lo = start.substr(0, length);
        string hi = end.substr(0, length);
        vector<int> result;

        for (const auto& [timestamp, id] : logs) {
            string prefix = timestamp.substr(0, length);
            if (lo <= prefix && prefix <= hi) {
                result.push_back(id);
            }
        }
        return result;
    }
};

int main() {
    LogSystem logs;
    string start = "2026:01:01:00:00:00";
    string end = "2026:01:01:12:00:00";

    assert(logs.retrieve(start, end, "Day").empty());

    logs.put(1, "2026:01:01:12:00:00");
    logs.put(2, "2026:01:01:23:59:59");
    logs.put(3, "2026:01:02:00:00:00");
    logs.put(4, "2026:01:01:12:00:00"); // Duplicate timestamp.

    vector<int> expected = {1, 2, 4};
    assert(logs.retrieve(start, end, "Day") == expected);
    // Target: the entire end day counts, regardless of end's hour.

    expected = {1, 4};
    assert(logs.retrieve(start, end, "Second") == expected);
    // Exact end is included; later seconds excluded.

    assert(logs.retrieve(
        "2027:01:01:00:00:00", end, "Year").empty());
    // Reversed range at the requested granularity.
}
