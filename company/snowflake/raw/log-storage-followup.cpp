#include <cassert>
#include <map>
#include <string>
#include <vector>

using std::multimap;
using std::string;
using std::vector;

int prefixLength(const string& granularity) {
    if (granularity == "Year") return 4;
    if (granularity == "Month") return 7;
    if (granularity == "Day") return 10;
    if (granularity == "Hour") return 13;
    if (granularity == "Minute") return 16;
    return 19;
}

class LogSystem {
private:
    multimap<string, int> logs; // ts -> id

public:
    // T: O(logn), S: O91)
    void put(int id, const string& timestamp) {
        logs.emplace(timestamp, id);
    }

    // T: O(logn+k), S: O(k)
    vector<int> retrieve(const string& start, const string& end,
                         const string& granularity) const {
        int length = prefixLength(granularity);
        string lowSuffix = "0000:00:00:00:00:00";
        string highSuffix = "9999:99:99:99:99:99";

        string lo = start.substr(0, length) + lowSuffix.substr(length);
        string hi = end.substr(0, length) + highSuffix.substr(length);

        vector<int> result;
        for (auto it = logs.lower_bound(lo);
             it != logs.end() && it->first <= hi; it++) {
            result.push_back(it->second);
        }
        return result;
    }
};

int main() {
    LogSystem logs;
    string start = "2026:01:01:00:00:00";
    string end = "2026:01:01:12:00:00";

    assert(logs.retrieve(start, end, "Day").empty());

    logs.put(2, "2026:01:01:23:59:59");
    logs.put(1, "2026:01:01:12:00:00"); // Out-of-order insertion.
    logs.put(3, "2026:01:02:00:00:00");
    logs.put(4, "2026:01:01:12:00:00"); // Duplicate timestamp.

    vector<int> expected = {1, 4, 2};
    assert(logs.retrieve(start, end, "Day") == expected);

    expected = {1, 4};
    assert(logs.retrieve(start, end, "Second") == expected);

    assert(logs.retrieve("2027:01:01:00:00:00", end, "Year").empty());
}
