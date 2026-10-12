#include <cassert>
#include <algorithm>
#include <iterator>
#include <map>
#include <string>
#include <unordered_map>

using std::map;
using std::string;
using std::unordered_map;

class TimeMap {
    unordered_map<string, map<int, string>> store; // key -> map<ts -> value>

public:
    // T: O(logn), S: O(1)
    void setValue(const string& key, const string& value, int timestamp) {
        store[key][timestamp] = value;
    }

    // T: O(logn), S: O(1)
    string get(const string& key, int timestamp) const {
        auto it = store.find(key);
        if (it == store.end()) { return ""; }

        const auto& versions = it->second;
        auto pos = versions.upper_bound(timestamp);

        if (pos == versions.begin()) { return ""; } // don't call std::prev(pos) when pos==versions.begin()
        return std::prev(pos)->second;
    }
};

int main() {
    TimeMap kv;
    kv.setValue("a", "late", 10);
    kv.setValue("a", "early", 5);

    assert(kv.get("a", 7) == "early");
    assert(kv.get("a", 10) == "late");

    kv.setValue("a", "fixed", 5);
    assert(kv.get("a", 5) == "fixed"); // same ts overwrites
    assert(kv.get("a", 12) == "late"); // other one unchanged
    assert(kv.get("a", 4).empty());
    assert(kv.get("missing", 7).empty());
}
