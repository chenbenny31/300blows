// set(key, value, timestamp); get(key, timestamp) returns the value set at the largest timestamp <= the given one,
// "" if none (LC981)
// J9934 follow-up: set timestamps can arrive out of order

// set timestamps increasing per key, or any order?
// same key and same timestamp twice: overwrite?
// get before the first set of a key, or an unknown key: ""?
// values non-empty, so "" means none?

#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <utility>
#include <algorithm>
#include <iterator>
#include <cassert>
#include <iostream>

using std::vector;
using std::string;
using std::pair;

// 1: timestamps increasing per key, append + binary search, T: set O(1), get O(log v), S: O(total sets)
class TimeMap_1 {
private:
    std::unordered_map<string, vector<pair<int, string>>> store; // key -> (timestamp, value), sorted by append order

public:
    void set(const string& key, const string& value, int timestamp) {
        store[key].push_back({timestamp, value});
    }

    string get(const string& key, int timestamp) {
        auto it = store.find(key);
        if (it == store.end()) {
            return "";
        }
        const auto& versions = it->second;
        auto pos = std::upper_bound(versions.begin(), versions.end(), timestamp,
                                    [](int t, const pair<int, string>& p) { return t < p.first; }); // first ts > t
        if (pos == versions.begin()) {
            return ""; // every version is newer than timestamp
        }
        return std::prev(pos)->second;
    }
};

// 2: any order, per-key ordered map, T: set O(log v), get O(log v), S: O(total sets)
class TimeMap_2 {
private:
    std::unordered_map<string, std::map<int, string>> store; // key -> timestamp -> value

public:
    void set(const string& key, const string& value, int timestamp) {
        store[key][timestamp] = value; // same timestamp overwrites
    }

    string get(const string& key, int timestamp) {
        auto it = store.find(key);
        if (it == store.end()) {
            return "";
        }
        const auto& versions = it->second;
        auto pos = versions.upper_bound(timestamp); // first ts > timestamp
        if (pos == versions.begin()) {
            return "";
        }
        return std::prev(pos)->second;
    }
};

int main() {
    TimeMap_1 a;
    assert(a.get("foo", 1) == ""); // null: unknown key
    a.set("foo", "bar", 1);
    a.set("foo", "bar2", 4);
    assert(a.get("foo", 3) == "bar"); // target: between versions, take the earlier one
    assert(a.get("foo", 4) == "bar2"); // edge: exact timestamp
    assert(a.get("foo", 0) == ""); // fault: before the first version

    TimeMap_2 b;
    assert(b.get("foo", 1) == ""); // null
    b.set("foo", "late", 10);
    b.set("foo", "early", 5); // out of order
    assert(b.get("foo", 7) == "early"); // target: order by timestamp, not by arrival
    assert(b.get("foo", 12) == "late");
    b.set("foo", "fixed", 5);
    assert(b.get("foo", 5) == "fixed"); // edge: same timestamp overwrites
    assert(b.get("foo", 4) == ""); // fault: before the first version

    std::cout << "all tests passed\n";
    return 0;
}
