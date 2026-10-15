#include <cassert>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using std::pair;
using std::string;
using std::unordered_map;
using std::vector;

class TimeMap {
    unordered_map<string, vector<pair<int, string>>> store; // key -> [(ts, value),]

public:
    // T: O(1), S: O(1)
    void setValue(const string& key, const string& value, int timestamp) {
        store[key].push_back({timestamp, value});
    }

    // T: O(logn), S: O(1)
    string get(const string& key, int timestamp) const {
        auto it = store.find(key);
        if (it == store.end()) { return ""; }

        const auto& versions = it->second;
        int lo = 0;
        int hi = versions.size(); // [0, n)

        while (lo < hi) { // upper_bound
            int mid = lo + (hi - lo) / 2;
            if (versions[mid].first <= timestamp) {
                lo = mid + 1;
            } else {
                hi = mid;
            }
        }
        return lo == 0 ? "" : versions[lo - 1].second;
    }
};

int main() {
    TimeMap tm;
    tm.setValue("a", "old", 1);
    tm.setValue("a", "new", 4);

    assert(tm.get("a", 3) == "old");
    assert(tm.get("a", 4) == "new");
    assert(tm.get("a", 9) == "new");
    assert(tm.get("a", 0).empty());
    assert(tm.get("missing", 3).empty());
}
